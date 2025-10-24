/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define O13_Pin GPIO_PIN_2
#define O13_GPIO_Port GPIOE
#define O14_Pin GPIO_PIN_3
#define O14_GPIO_Port GPIOE
#define O15_Pin GPIO_PIN_4
#define O15_GPIO_Port GPIOE
#define O16_Pin GPIO_PIN_5
#define O16_GPIO_Port GPIOE
#define O17_Pin GPIO_PIN_6
#define O17_GPIO_Port GPIOE
#define O18_Pin GPIO_PIN_13
#define O18_GPIO_Port GPIOC
#define I1_Pin GPIO_PIN_0
#define I1_GPIO_Port GPIOC
#define I2_Pin GPIO_PIN_1
#define I2_GPIO_Port GPIOC
#define I3_Pin GPIO_PIN_2
#define I3_GPIO_Port GPIOC
#define I4_Pin GPIO_PIN_3
#define I4_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOA
#define I5_Pin GPIO_PIN_4
#define I5_GPIO_Port GPIOC
#define I6_Pin GPIO_PIN_5
#define I6_GPIO_Port GPIOC
#define I7_Pin GPIO_PIN_0
#define I7_GPIO_Port GPIOB
#define I8_Pin GPIO_PIN_1
#define I8_GPIO_Port GPIOB
#define I9_Pin GPIO_PIN_7
#define I9_GPIO_Port GPIOE
#define I10_Pin GPIO_PIN_8
#define I10_GPIO_Port GPIOE
#define I11_Pin GPIO_PIN_9
#define I11_GPIO_Port GPIOE
#define I12_Pin GPIO_PIN_10
#define I12_GPIO_Port GPIOE
#define I13_Pin GPIO_PIN_11
#define I13_GPIO_Port GPIOE
#define I14_Pin GPIO_PIN_12
#define I14_GPIO_Port GPIOE
#define I15_Pin GPIO_PIN_13
#define I15_GPIO_Port GPIOE
#define I16_Pin GPIO_PIN_14
#define I16_GPIO_Port GPIOE
#define I17_Pin GPIO_PIN_15
#define I17_GPIO_Port GPIOE
#define I18_Pin GPIO_PIN_12
#define I18_GPIO_Port GPIOB
#define DIR2_CH1_Pin GPIO_PIN_8
#define DIR2_CH1_GPIO_Port GPIOD
#define DIR2_CH2_Pin GPIO_PIN_9
#define DIR2_CH2_GPIO_Port GPIOD
#define DIR2_CH3_Pin GPIO_PIN_10
#define DIR2_CH3_GPIO_Port GPIOD
#define DIR2_CH4_Pin GPIO_PIN_11
#define DIR2_CH4_GPIO_Port GPIOD
#define DIR1_CH1_Pin GPIO_PIN_7
#define DIR1_CH1_GPIO_Port GPIOC
#define DIR1_CH2_Pin GPIO_PIN_9
#define DIR1_CH2_GPIO_Port GPIOC
#define DIR1_CH3_Pin GPIO_PIN_9
#define DIR1_CH3_GPIO_Port GPIOA
#define DIR1_CH4_Pin GPIO_PIN_15
#define DIR1_CH4_GPIO_Port GPIOA
#define O1_Pin GPIO_PIN_3
#define O1_GPIO_Port GPIOD
#define O2_Pin GPIO_PIN_4
#define O2_GPIO_Port GPIOD
#define O3_Pin GPIO_PIN_5
#define O3_GPIO_Port GPIOD
#define O4_Pin GPIO_PIN_6
#define O4_GPIO_Port GPIOD
#define O5_Pin GPIO_PIN_7
#define O5_GPIO_Port GPIOD
#define O6_Pin GPIO_PIN_3
#define O6_GPIO_Port GPIOB
#define O7_Pin GPIO_PIN_4
#define O7_GPIO_Port GPIOB
#define O8_Pin GPIO_PIN_5
#define O8_GPIO_Port GPIOB
#define O9_Pin GPIO_PIN_8
#define O9_GPIO_Port GPIOB
#define O10_Pin GPIO_PIN_9
#define O10_GPIO_Port GPIOB
#define O11_Pin GPIO_PIN_0
#define O11_GPIO_Port GPIOE
#define O12_Pin GPIO_PIN_1
#define O12_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

// ===== Modbus Coil Definitions =====
// Ánh xạ 6 coils để điều khiển 3 trục servo (X, Y, Z) mỗi trục 2 hướng
// Coils_Database là mảng byte từ modbusSlave.h, mỗi bit = 1 coil
// Địa chỉ trực tiếp trong Coils_Database[] (byte index)
#define COIL_X_NEG  3  // X trái  → Modbus Coil 4  (Coils_Database[0] bit 3)
#define COIL_X_POS  4  // X phải  → Modbus Coil 5  (Coils_Database[0] bit 4)
#define COIL_Y_POS  5  // Y lên   → Modbus Coil 6  (Coils_Database[0] bit 5)
#define COIL_Y_NEG  6  // Y xuống → Modbus Coil 7  (Coils_Database[0] bit 6)
#define COIL_Z_POS  7  // Z lên   → Modbus Coil 8  (Coils_Database[0] bit 7)
#define COIL_Z_NEG  8  // Z xuống → Modbus Coil 9  (Coils_Database[1] bit 0)

// ===== Coil Access Macro =====
// Macro để đọc bit từ Coils_Database
// Chia địa chỉ cho 8 để lấy byte, dùng modulo để lấy vị trí bit, shift và AND để lấy giá trị
#define GET_COIL(addr) ((Coils_Database[(addr)/8] >> ((addr)%8)) & 0x01)

// ===== Modbus Library External Declarations =====
// Coils_Database and Holding_Registers_Database are defined in modbusSlave.h (static)
// They are included via modbusSlave.h -> modbus_handler.h includes
extern UART_HandleTypeDef huart2;

// ===== PWM CONFIGURATION =====
// Tốc độ PWM cố định 10kHz (PWM timers already configured in MX_TIMx_Init)
#define PWM_FREQ_10KHZ  10

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
