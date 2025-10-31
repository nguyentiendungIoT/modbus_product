/*
 * motor_control.c
 *
 * Implementation of simple motor control previously living in main.c
 */

#include "motor_control.h"
#include <string.h>

// The timers and GPIOs are defined in main.c; declare them extern here
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim9;

// Motor X: PWM TIM1_CH1, counter TIM2, DIR pin: DIR1_CH1_Pin / DIR1_CH1_GPIO_Port (PC6)
void Motor_X_Move(uint8_t direction, uint32_t pulses)
{
  // Set direction
  if (direction == 0) {
    HAL_GPIO_WritePin(DIR1_CH1_GPIO_Port, DIR1_CH1_Pin, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(DIR1_CH1_GPIO_Port, DIR1_CH1_Pin, GPIO_PIN_SET);
  }

  if (pulses == 0) {
    // continuous mode
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  } else {
    __HAL_TIM_SET_AUTORELOAD(&htim2, (pulses - 1));
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  }
}

// Motor Y: PWM TIM8_CH1, counter TIM5, DIR pin: DIR1_CH2_Pin / DIR1_CH2_GPIO_Port (PC7)
void Motor_Y_Move(uint8_t direction, uint32_t pulses)
{
  if (direction == 0) {
    HAL_GPIO_WritePin(DIR1_CH2_GPIO_Port, DIR1_CH2_Pin, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(DIR1_CH2_GPIO_Port, DIR1_CH2_Pin, GPIO_PIN_SET);
  }

  if (pulses == 0) {
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  } else {
    __HAL_TIM_SET_AUTORELOAD(&htim5, (pulses - 1));
    __HAL_TIM_SET_COUNTER(&htim5, 0);
    HAL_TIM_Base_Start_IT(&htim5);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  }
}

// Motor Z: PWM TIM3_CH3, counter TIM9, DIR pin: DIR1_CH3_Pin / DIR1_CH3_GPIO_Port (PA9? configured as PC9 earlier)
// Using DIR1_CH3_Pin/Port from main.h mapping (user project pins)
void Motor_Z_Move(uint8_t direction, uint32_t pulses)
{
  if (direction == 0) {
    HAL_GPIO_WritePin(DIR1_CH3_GPIO_Port, DIR1_CH3_Pin, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(DIR1_CH3_GPIO_Port, DIR1_CH3_Pin, GPIO_PIN_SET);
  }

  if (pulses == 0) {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  } else {
    __HAL_TIM_SET_AUTORELOAD(&htim9, (pulses - 1));
    __HAL_TIM_SET_COUNTER(&htim9, 0);
    HAL_TIM_Base_Start_IT(&htim9);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  }
}

void Motor_X_Stop(void)
{
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void Motor_Y_Stop(void)
{
  HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
}

void Motor_Z_Stop(void)
{
  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
}

void Motor_All_Stop(void)
{
  Motor_X_Stop();
  Motor_Y_Stop();
  Motor_Z_Stop();
}

// Timer callback to stop motors for pulse-limited mode
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    Motor_X_Stop();
  }
  else if (htim->Instance == TIM5) {
    Motor_Y_Stop();
  }
  else if (htim->Instance == TIM9) {
    Motor_Z_Stop();
  }
}
