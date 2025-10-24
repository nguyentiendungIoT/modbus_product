/*
 * motor_control.h
 *
 * Thin motor control API separated from main.c
 */
#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

// pulses == 0 => continuous mode (run until Stop called)
#define PWM_FREQ_10KHZ  10

void Motor_X_Move(uint8_t direction, uint32_t pulses);
void Motor_Y_Move(uint8_t direction, uint32_t pulses);
void Motor_Z_Move(uint8_t direction, uint32_t pulses);
void Motor_X_Stop(void);
void Motor_Y_Stop(void);
void Motor_Z_Stop(void);
void Motor_All_Stop(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_MOTOR_CONTROL_H_ */
