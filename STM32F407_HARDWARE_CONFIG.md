# STM32F407 - Hardware Configuration cho Pulse Control

## 📌 Pin Configuration

### **TIM1 - PWM Output (Phát Xung)**

```
┌─────────────────────────────────────┐
│         TIM1 - PWM Output           │
├─────────────────────────────────────┤
│ Channel │ Pin      │ Alternate Func  │
├─────────┼──────────┼─────────────────┤
│ CH1     │ PA8      │ AF1             │
│ CH2     │ PA9      │ AF1             │
│ CH3     │ PA10     │ AF1             │
│ CH4     │ PA11     │ AF1             │
│ CH1N    │ PB13     │ AF1             │
│ CH2N    │ PB14     │ AF1             │
│ CH3N    │ PB15     │ AF1             │
│ BKIN    │ PB12     │ AF1             │
└─────────────────────────────────────┘
```

**Khuyến nghị cho X-Axis**: PA8 (TIM1_CH1)

---

### **TIM2 - Pulse Counter (Đếm Xung)**

```
┌─────────────────────────────────────┐
│     TIM2 - Input Capture Counter    │
├─────────────────────────────────────┤
│ Channel │ Pin      │ Alternate Func  │
├─────────┼──────────┼─────────────────┤
│ CH1     │ PA0      │ AF1             │
│ CH2     │ PA1      │ AF1             │
│ CH3     │ PA2      │ AF1             │
│ CH4     │ PA3      │ AF1             │
│ TI1_ETR │ PA0      │ AF1             │
└─────────────────────────────────────┘
```

**Khuyến nghị**: PA0 (TIM2_CH1) - Đếm xung từ PA8

---

### **TIM3 - PWM Y-Axis**

```
┌─────────────────────────────────────┐
│         TIM3 - PWM Output           │
├─────────────────────────────────────┤
│ Channel │ Pin      │ Alternate Func  │
├─────────┼──────────┼─────────────────┤
│ CH1     │ PA6      │ AF2             │
│ CH1     │ PB4      │ AF2             │
│ CH2     │ PA7      │ AF2             │
│ CH2     │ PB5      │ AF2             │
│ CH3     │ PB0      │ AF2             │
│ CH4     │ PB1      │ AF2             │
└─────────────────────────────────────┘
```

**Khuyến nghị**: PB8 (TIM3_CH3)

---

### **TIM5 - Pulse Counter Y-Axis**

```
┌─────────────────────────────────────┐
│     TIM5 - Input Capture Counter    │
├─────────────────────────────────────┤
│ Channel │ Pin      │ Alternate Func  │
├─────────┼──────────┼─────────────────┤
│ CH1     │ PA0      │ AF2             │
│ CH2     │ PA1      │ AF2             │
│ CH3     │ PA2      │ AF2             │
│ CH4     │ PA3      │ AF2             │
└─────────────────────────────────────┘
```

---

### **TIM8 - PWM Z-Axis**

```
┌─────────────────────────────────────┐
│         TIM8 - PWM Output           │
├─────────────────────────────────────┤
│ Channel │ Pin      │ Alternate Func  │
├─────────┼──────────┼─────────────────┤
│ CH1     │ PC6      │ AF3             │
│ CH2     │ PC7      │ AF3             │
│ CH3     │ PC8      │ AF3             │
│ CH4     │ PC9      │ AF3             │
└─────────────────────────────────────┘
```

---

### **TIM9 - Pulse Counter Z-Axis**

```
┌─────────────────────────────────────┐
│     TIM9 - Input Capture Counter    │
├─────────────────────────────────────┤
│ Channel │ Pin      │ Alternate Func  │
├─────────┼──────────┼─────────────────┤
│ CH1     │ PA2      │ AF3             │
│ CH2     │ PA3      │ AF3             │
└─────────────────────────────────────┘
```

---

## 🔌 Hardware Wiring - 3 Axes

### **Phương Án 1: PWM Direct + GPIO Direction (Hiện Tại)**

```
┌──────────────────────────────────────────────────────────┐
│                    STM32F407VET6                          │
├──────────────────────────────────────────────────────────┤
│                                                            │
│  X-AXIS CONTROL:                                         │
│  ├─ PA8  (TIM1_CH1)   → PWM output → Motor Driver IN    │
│  ├─ PA0  (TIM2_CH1)   ← Encoder pulse input (optional)  │
│  ├─ PC0  (GPIO)       → Direction control (DIR1_X)     │
│  └─ PC1  (GPIO)       → Direction control (DIR2_X)     │
│                                                            │
│  Y-AXIS CONTROL:                                         │
│  ├─ PC6  (TIM8_CH1)   → PWM output → Motor Driver IN    │
│  ├─ PA1  (TIM5_CH1)   ← Encoder pulse input (optional)  │
│  ├─ PC2  (GPIO)       → Direction control (DIR1_Y)     │
│  └─ PC3  (GPIO)       → Direction control (DIR2_Y)     │
│                                                            │
│  Z-AXIS CONTROL:                                         │
│  ├─ PB8  (TIM3_CH3)   → PWM output → Motor Driver IN    │
│  ├─ PA2  (TIM9_CH1)   ← Encoder pulse input (optional)  │
│  ├─ PC4  (GPIO)       → Direction control (DIR1_Z)     │
│  └─ PC5  (GPIO)       → Direction control (DIR2_Z)     │
│                                                            │
│  UART (Modbus):                                          │
│  ├─ PA2  (USART2_TX)  → RS485 Module TX                 │
│  └─ PA3  (USART2_RX)  → RS485 Module RX                 │
│                                                            │
└──────────────────────────────────────────────────────────┘
```

---

### **Phương Án 2: Encoder Mode (Nếu có Encoder A/B)**

```
┌──────────────────────────────────────────────────────────┐
│                    STM32F407VET6                          │
├──────────────────────────────────────────────────────────┤
│                                                            │
│  X-AXIS:                                                 │
│  ├─ PA8  (TIM1_CH1)   → PWM output                       │
│  ├─ PA0  (TIM2_CH1)   ← Encoder A signal                 │
│  ├─ PA1  (TIM2_CH2)   ← Encoder B signal                 │
│  ├─ PC0  (GPIO)       → (Optional) Direction control    │
│  └─ PC1  (GPIO)       → (Optional) Motor Enable         │
│                                                            │
│  Y-AXIS:                                                 │
│  ├─ PC6  (TIM8_CH1)   → PWM output                       │
│  ├─ PA2  (TIM5_CH1)   ← Encoder A signal                 │
│  ├─ PA3  (TIM5_CH2)   ← Encoder B signal                 │
│  └─ ...                                                   │
│                                                            │
│  Z-AXIS:                                                 │
│  ├─ PB8  (TIM3_CH3)   → PWM output                       │
│  ├─ PC0  (TIM3_CH1)   ← Encoder A signal                 │
│  ├─ PC1  (TIM3_CH2)   ← Encoder B signal                 │
│  └─ ...                                                   │
│                                                            │
└──────────────────────────────────────────────────────────┘
```

---

## 🎛️ GPIO Configuration Code

### **X-Axis Configuration**

```c
#include "stm32f4xx_hal.h"

/* GPIO Definitions */
#define X_PWM_PORT      GPIOA
#define X_PWM_PIN       GPIO_PIN_8      // TIM1_CH1
#define X_ENCODER_PORT  GPIOA
#define X_ENCODER_PIN   GPIO_PIN_0      // TIM2_CH1
#define X_DIR1_PORT     GPIOC
#define X_DIR1_PIN      GPIO_PIN_0
#define X_DIR2_PORT     GPIOC
#define X_DIR2_PIN      GPIO_PIN_1

/**
 * @brief Cấu hình GPIO cho X-Axis
 */
void X_Axis_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* PA8 - PWM Output (TIM1_CH1) */
    GPIO_InitStruct.Pin = X_PWM_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(X_PWM_PORT, &GPIO_InitStruct);
    
    /* PA0 - Input Capture (TIM2_CH1) */
    GPIO_InitStruct.Pin = X_ENCODER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;  // Pull down để lọc nhiễu
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(X_ENCODER_PORT, &GPIO_InitStruct);
    
    /* PC0, PC1 - Direction GPIO */
    GPIO_InitStruct.Pin = X_DIR1_PIN | X_DIR2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(X_DIR1_PORT, &GPIO_InitStruct);
    
    /* Default: Dir1 = 0, Dir2 = 0 */
    HAL_GPIO_WritePin(X_DIR1_PORT, X_DIR1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(X_DIR2_PORT, X_DIR2_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Set X-Axis direction
 * @param direction: 1 = forward, 0 = backward
 */
void X_Axis_SetDirection(uint8_t direction)
{
    if (direction) {
        HAL_GPIO_WritePin(X_DIR1_PORT, X_DIR1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(X_DIR2_PORT, X_DIR2_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(X_DIR1_PORT, X_DIR1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(X_DIR2_PORT, X_DIR2_PIN, GPIO_PIN_SET);
    }
}
```

---

### **Y-Axis Configuration**

```c
#define Y_PWM_PORT      GPIOC
#define Y_PWM_PIN       GPIO_PIN_6      // TIM8_CH1
#define Y_ENCODER_PORT  GPIOA
#define Y_ENCODER_PIN   GPIO_PIN_1      // TIM5_CH1
#define Y_DIR1_PORT     GPIOC
#define Y_DIR1_PIN      GPIO_PIN_2
#define Y_DIR2_PORT     GPIOC
#define Y_DIR2_PIN      GPIO_PIN_3

void Y_Axis_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* PC6 - PWM Output (TIM8_CH1) */
    GPIO_InitStruct.Pin = Y_PWM_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(Y_PWM_PORT, &GPIO_InitStruct);
    
    /* PA1 - Input Capture (TIM5_CH1) */
    GPIO_InitStruct.Pin = Y_ENCODER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(Y_ENCODER_PORT, &GPIO_InitStruct);
    
    /* PC2, PC3 - Direction GPIO */
    GPIO_InitStruct.Pin = Y_DIR1_PIN | Y_DIR2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Y_DIR1_PORT, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(Y_DIR1_PORT, Y_DIR1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Y_DIR2_PORT, Y_DIR2_PIN, GPIO_PIN_RESET);
}

void Y_Axis_SetDirection(uint8_t direction)
{
    if (direction) {
        HAL_GPIO_WritePin(Y_DIR1_PORT, Y_DIR1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Y_DIR2_PORT, Y_DIR2_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(Y_DIR1_PORT, Y_DIR1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Y_DIR2_PORT, Y_DIR2_PIN, GPIO_PIN_SET);
    }
}
```

---

### **Z-Axis Configuration**

```c
#define Z_PWM_PORT      GPIOB
#define Z_PWM_PIN       GPIO_PIN_8      // TIM3_CH3
#define Z_ENCODER_PORT  GPIOA
#define Z_ENCODER_PIN   GPIO_PIN_2      // TIM9_CH1
#define Z_DIR1_PORT     GPIOC
#define Z_DIR1_PIN      GPIO_PIN_4
#define Z_DIR2_PORT     GPIOC
#define Z_DIR2_PIN      GPIO_PIN_5

void Z_Axis_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* PB8 - PWM Output (TIM3_CH3) */
    GPIO_InitStruct.Pin = Z_PWM_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(Z_PWM_PORT, &GPIO_InitStruct);
    
    /* PA2 - Input Capture (TIM9_CH1) */
    GPIO_InitStruct.Pin = Z_ENCODER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
    HAL_GPIO_Init(Z_ENCODER_PORT, &GPIO_InitStruct);
    
    /* PC4, PC5 - Direction GPIO */
    GPIO_InitStruct.Pin = Z_DIR1_PIN | Z_DIR2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Z_DIR1_PORT, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(Z_DIR1_PORT, Z_DIR1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Z_DIR2_PORT, Z_DIR2_PIN, GPIO_PIN_RESET);
}

void Z_Axis_SetDirection(uint8_t direction)
{
    if (direction) {
        HAL_GPIO_WritePin(Z_DIR1_PORT, Z_DIR1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Z_DIR2_PORT, Z_DIR2_PIN, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(Z_DIR1_PORT, Z_DIR1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Z_DIR2_PORT, Z_DIR2_PIN, GPIO_PIN_SET);
    }
}
```

---

## 📋 Initialization Summary

```c
void Hardware_Init(void)
{
    HAL_Init();
    SystemClock_Config();
    
    /* GPIO Configuration */
    X_Axis_GPIO_Init();
    Y_Axis_GPIO_Init();
    Z_Axis_GPIO_Init();
    
    /* Timer Configuration */
    X_PWM_Init();         // TIM1
    X_Counter_Init();     // TIM2
    Y_PWM_Init();         // TIM8
    Y_Counter_Init();     // TIM5
    Z_PWM_Init();         // TIM3
    Z_Counter_Init();     // TIM9
    
    /* UART Configuration */
    USART2_Init();        // Modbus
}
```

---

## 🎯 Kiểm Tra Hardware

```bash
# 1. Dùng multimeter kiểm tra:
# - PA8, PC6, PB8 có +5V (khi PWM chạy)
# - PC0-PC5 thay đổi giữa 0V và +5V (direction)

# 2. Dùng oscilloscope:
# - PA8 (TIM1_CH1): 10kHz PWM, duty tuỳ chỉnh
# - PC6 (TIM8_CH1): 10kHz PWM, duty tuỳ chỉnh
# - PB8 (TIM3_CH3): 10kHz PWM, duty tuỳ chỉnh

# 3. Dùng logic analyzer:
# - PA0 (TIM2_CH1): Bắt xung từ encoder
# - PA1 (TIM5_CH1): Bắt xung từ encoder
# - PA2 (TIM9_CH1): Bắt xung từ encoder
```

---

## ✅ Pin Summary Table

| Chức Năng | Port | Pin | Alternate | Dùng Cho |
|-----------|------|-----|-----------|----------|
| X_PWM | PA | 8 | AF1 (TIM1_CH1) | Motor PWM |
| X_Encoder | PA | 0 | AF1 (TIM2_CH1) | Pulse Counter |
| X_Dir1 | PC | 0 | GPIO | Direction |
| X_Dir2 | PC | 1 | GPIO | Direction |
| Y_PWM | PC | 6 | AF3 (TIM8_CH1) | Motor PWM |
| Y_Encoder | PA | 1 | AF2 (TIM5_CH1) | Pulse Counter |
| Y_Dir1 | PC | 2 | GPIO | Direction |
| Y_Dir2 | PC | 3 | GPIO | Direction |
| Z_PWM | PB | 8 | AF2 (TIM3_CH3) | Motor PWM |
| Z_Encoder | PA | 2 | AF3 (TIM9_CH1) | Pulse Counter |
| Z_Dir1 | PC | 4 | GPIO | Direction |
| Z_Dir2 | PC | 5 | GPIO | Direction |

