# STM32F407 Timer GPIO Pin Mapping

## Tổng quan Timer STM32F407

STM32F407 có **14 timers** với các chức năng khác nhau:
- **TIM1, TIM8**: Advanced timers (16-bit, 3-phase PWM)
- **TIM2, TIM3, TIM4, TIM5**: General-purpose timers (16-bit/32-bit)
- **TIM6, TIM7**: Basic timers (16-bit, no I/O)
- **TIM9, TIM10, TIM11, TIM12, TIM13, TIM14**: General-purpose timers (16-bit)

---

## TIM1 (Advanced Timer) - GPIO Mapping

**TIM1_CH1 (PWM Output)**
- PA8 (AF1)
- PE9 (AF1)

**TIM1_CH2 (PWM Output)**
- PA9 (AF1)
- PE11 (AF1)

**TIM1_CH3 (PWM Output)**
- PA10 (AF1)
- PE13 (AF1)

**TIM1_CH4 (PWM Output)**
- PA11 (AF1)
- PE14 (AF1)

**TIM1_ETR (External Trigger)**
- PA12 (AF1)
- PE7 (AF1)

---

## TIM2 (General Purpose Timer) - GPIO Mapping

**TIM2_CH1 (PWM/Input Capture)**
- PA0 (AF1)
- PA5 (AF1)
- PA15 (AF1)

**TIM2_CH2 (PWM/Input Capture)**
- PA1 (AF1)
- PB3 (AF1)

**TIM2_CH3 (PWM/Input Capture)**
- PA2 (AF1)
- PB10 (AF1)

**TIM2_CH4 (PWM/Input Capture)**
- PA3 (AF1)
- PB11 (AF1)

**TIM2_ETR (External Trigger)**
- PA0 (AF1)
- PA5 (AF1)
- PA15 (AF1)

---

## TIM3 (General Purpose Timer) - GPIO Mapping

**TIM3_CH1 (PWM/Input Capture)**
- PA6 (AF2)
- PB4 (AF2)
- PC6 (AF2)

**TIM3_CH2 (PWM/Input Capture)**
- PA7 (AF2)
- PB5 (AF2)
- PC7 (AF2)

**TIM3_CH3 (PWM/Input Capture)**
- PB0 (AF2)
- PC8 (AF2)

**TIM3_CH4 (PWM/Input Capture)**
- PB1 (AF2)
- PC9 (AF2)

**TIM3_ETR (External Trigger)**
- PD2 (AF2)

---

## TIM4 (General Purpose Timer) - GPIO Mapping

**TIM4_CH1 (PWM/Input Capture)**
- PB6 (AF2)
- PD12 (AF2)

**TIM4_CH2 (PWM/Input Capture)**
- PB7 (AF2)
- PD13 (AF2)

**TIM4_CH3 (PWM/Input Capture)**
- PB8 (AF2)
- PD14 (AF2)

**TIM4_CH4 (PWM/Input Capture)**
- PB9 (AF2)
- PD15 (AF2)

**TIM4_ETR (External Trigger)**
- PE0 (AF2)

---

## TIM5 (General Purpose Timer) - GPIO Mapping

**TIM5_CH1 (PWM/Input Capture)**
- PA0 (AF2)
- PH10 (AF2)

**TIM5_CH2 (PWM/Input Capture)**
- PA1 (AF2)
- PH11 (AF2)

**TIM5_CH3 (PWM/Input Capture)**
- PA2 (AF2)
- PH12 (AF2)

**TIM5_CH4 (PWM/Input Capture)**
- PA3 (AF2)
- PI0 (AF2)

**TIM5_ETR (External Trigger)**
- PF6 (AF2)
- PH13 (AF2)

---

## TIM8 (Advanced Timer) - GPIO Mapping

**TIM8_CH1 (PWM Output)**
- PC6 (AF3)
- PI5 (AF3)

**TIM8_CH2 (PWM Output)**
- PC7 (AF3)
- PI6 (AF3)

**TIM8_CH3 (PWM Output)**
- PC8 (AF3)
- PI7 (AF3)

**TIM8_CH4 (PWM Output)**
- PC9 (AF3)
- PI2 (AF3)

**TIM8_ETR (External Trigger)**
- PA0 (AF3)
- PI3 (AF3)

---

## TIM9 (General Purpose Timer) - GPIO Mapping

**TIM9_CH1 (PWM/Input Capture)**
- PA2 (AF3)
- PE5 (AF3)

**TIM9_CH2 (PWM/Input Capture)**
- PA3 (AF3)
- PE6 (AF3)

---

## TIM10 (General Purpose Timer) - GPIO Mapping

**TIM10_CH1 (PWM/Input Capture)**
- PB8 (AF3)
- PF6 (AF3)

---

## TIM11 (General Purpose Timer) - GPIO Mapping

**TIM11_CH1 (PWM/Input Capture)**
- PB9 (AF3)
- PF7 (AF3)

---

## TIM12 (General Purpose Timer) - GPIO Mapping

**TIM12_CH1 (PWM/Input Capture)**
- PB14 (AF9)
- PH6 (AF9)

**TIM12_CH2 (PWM/Input Capture)**
- PB15 (AF9)
- PH9 (AF9)

---

## TIM13 (General Purpose Timer) - GPIO Mapping

**TIM13_CH1 (PWM/Input Capture)**
- PA6 (AF9)
- PF8 (AF9)

---

## TIM14 (General Purpose Timer) - GPIO Mapping

**TIM14_CH1 (PWM/Input Capture)**
- PA7 (AF9)
- PF9 (AF9)

---

## Encoder Interface Pins

### TIM2 Encoder Mode
- **Channel 1**: PA0, PA5, PA15
- **Channel 2**: PA1, PB3

### TIM3 Encoder Mode
- **Channel 1**: PA6, PB4, PC6
- **Channel 2**: PA7, PB5, PC7

### TIM4 Encoder Mode
- **Channel 1**: PB6, PD12
- **Channel 2**: PB7, PD13

### TIM5 Encoder Mode
- **Channel 1**: PA0, PH10
- **Channel 2**: PA1, PH11

---

## PWM Output Pins (Most Common)

### X-Axis PWM (TIM1_CH1)
- **PA8** (AF1) - Recommended
- PE9 (AF1)

### Y-Axis PWM (TIM8_CH1)
- **PC6** (AF3) - Recommended
- PI5 (AF3)

### Z-Axis PWM (TIM3_CH3)
- **PB0** (AF2) - Recommended
- PC8 (AF2)

---

## Encoder Input Pins (Most Common)

### X-Axis Encoder (TIM2)
- **PA0** (TIM2_CH1) - Recommended
- **PA1** (TIM2_CH2) - Recommended

### Y-Axis Encoder (TIM5)
- **PA2** (TIM5_CH1) - Recommended
- **PA3** (TIM5_CH2) - Recommended

### Z-Axis Encoder (TIM3)
- **PB4** (TIM3_CH1) - Recommended
- **PB5** (TIM3_CH2) - Recommended

---

## Direction Control GPIO Pins

### X-Axis Direction
- **DIR1_X**: PB12 (GPIO_Output)
- **DIR2_X**: PB13 (GPIO_Output)

### Y-Axis Direction
- **DIR1_Y**: PB14 (GPIO_Output)
- **DIR2_Y**: PB15 (GPIO_Output)

### Z-Axis Direction
- **DIR1_Z**: PC10 (GPIO_Output)
- **DIR2_Z**: PC11 (GPIO_Output)

---

## Notes

1. **AF (Alternate Function)**: Số AF chỉ định alternate function mode
2. **Recommended Pins**: Được đánh dấu **bold** là các chân được sử dụng trong code hiện tại
3. **Multiple Options**: Nhiều chân có thể được sử dụng cho cùng một timer channel
4. **Pin Conflicts**: Một số chân có thể được sử dụng cho nhiều peripheral khác nhau
5. **Power Pins**: Không sử dụng các chân nguồn (VDD, GND) làm GPIO

---

## Current Project Pin Usage

```c
// PWM Outputs
#define PWM_X_PIN       GPIO_PIN_8   // PA8 - TIM1_CH1
#define PWM_Y_PIN       GPIO_PIN_6   // PC6 - TIM8_CH1
#define PWM_Z_PIN       GPIO_PIN_0   // PB0 - TIM3_CH3

// Encoder Inputs
#define ENC_X_A_PIN     GPIO_PIN_0   // PA0 - TIM2_CH1
#define ENC_X_B_PIN     GPIO_PIN_1   // PA1 - TIM2_CH2
#define ENC_Y_A_PIN     GPIO_PIN_2   // PA2 - TIM5_CH1
#define ENC_Y_B_PIN     GPIO_PIN_3   // PA3 - TIM5_CH2
#define ENC_Z_A_PIN     GPIO_PIN_4   // PB4 - TIM3_CH1
#define ENC_Z_B_PIN     GPIO_PIN_5   // PB5 - TIM3_CH2

// Direction Control
#define DIR1_X_PIN      GPIO_PIN_12  // PB12
#define DIR2_X_PIN      GPIO_PIN_13  // PB13
#define DIR1_Y_PIN      GPIO_PIN_14  // PB14
#define DIR2_Y_PIN      GPIO_PIN_15  // PB15
#define DIR1_Z_PIN      GPIO_PIN_10  // PC10
#define DIR2_Z_PIN      GPIO_PIN_11  // PC11
```

---

## GPIO Port Mapping

| Port | Pins Used | Functions |
|------|-----------|-----------|
| **PA** | PA0, PA1, PA2, PA3, PA8 | Encoder inputs, PWM X |
| **PB** | PB0, PB4, PB5, PB12, PB13, PB14, PB15 | PWM Z, Encoder Z, Direction X/Y |
| **PC** | PC6, PC10, PC11 | PWM Y, Direction Z |

---

## Alternate Function Values

| Timer | AF Value | Description |
|-------|----------|-------------|
| TIM1 | AF1 | Advanced timer |
| TIM2 | AF1 | General purpose |
| TIM3 | AF2 | General purpose |
| TIM4 | AF2 | General purpose |
| TIM5 | AF2 | General purpose |
| TIM8 | AF3 | Advanced timer |
| TIM9 | AF3 | General purpose |
| TIM10 | AF3 | General purpose |
| TIM11 | AF3 | General purpose |
| TIM12 | AF9 | General purpose |
| TIM13 | AF9 | General purpose |
| TIM14 | AF9 | General purpose |

---

## References

- STM32F407VG Datasheet (DS8626)
- STM32F4xx Reference Manual (RM0090)
- STM32F4xx HAL Driver Documentation</content>
<parameter name="filePath">d:\workspace_cubeide_project\test\test\STM32F407_GPIO_TIMER_MAPPING.md