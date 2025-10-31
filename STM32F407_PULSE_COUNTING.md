# STM32F407 - Các Cách Điều Khiển Chính Xác Số Xung (Pulse Counting)

## 📌 Tóm Tắt

STM32F407 có nhiều chức năng hỗ trợ **điều khiển chính xác số xung**. Dựa trên datasheet và STM32 HAL Driver, có **4 phương pháp chính** để đếm và kiểm soát số xung:

---

## 🎯 4 Phương Pháp Chính

### **1. TIMER COUNTER MODE (Chế Độ Đếm Cơ Bản) ⭐ Đơn Giản**

**Nguyên Lý**: Sử dụng thanh ghi đếm (CNT) của Timer để đếm xung từ GPIO hoặc tín hiệu ngoài.

**Hỗ Trợ**:
- ✅ Tất cả Timer trên STM32F407 (TIM2, TIM3, TIM5, TIM9, TIM10, TIM11, v.v.)
- Chế độ đếm: UP, DOWN, CENTER-ALIGNED (3 loại)
- Prescaler có thể điều chỉnh
- Thanh ghi CNT 16-bit (0-65535)

**Ưu Điểm**:
- Đơn giản, dễ sử dụng
- Không cần interrupt (nếu chỉ muốn đếm)
- Tốn ít tài nguyên

**Nhược Điểm**:
- Không thể đếm ngược/thuận tự động
- Cần kiểm tra overflow thủ công

**Cách Sử Dụng**:
```c
// Cấu hình Timer ở chế độ base
TIM_HandleTypeDef htim2;
htim2.Instance = TIM2;
htim2.Init.Period = 65535;  // Max count
htim2.Init.Prescaler = 0;
htim2.Init.CounterMode = TIM_COUNTERMODE_UP;  // Đếm tăng
HAL_TIM_Base_Init(&htim2);

// Khởi động đếm
HAL_TIM_Base_Start(&htim2);

// Đọc giá trị đếm hiện tại
uint16_t pulse_count = __HAL_TIM_GET_COUNTER(&htim2);

// Đặt lại giá trị đếm
__HAL_TIM_SET_COUNTER(&htim2, 0);
```

**STM32F407 Timer hỗ trợ**: TIM2, TIM3, TIM4, TIM5 (32-bit) có độ chính xác tốt

---

### **2. INPUT CAPTURE MODE (Chế Độ Bắt Tín Hiệu Nhập) ⭐ Chính Xác**

**Nguyên Lý**: Dùng chức năng Input Capture để bắt thời điểm cạnh lên/xuống và tính thời gian giữa các xung.

**Hỗ Trợ**:
- ✅ Tất cả Timer (TIM1, TIM2, TIM3, TIM4, TIM5, TIM8, TIM9, TIM10, TIM11)
- Bắt tín hiệu trên 4 channel (CH1-CH4)
- Có thể thiết lập lọc (ICFilter) và prescaler (ICPrescaler)
- Có interrupt khi bắt được xung

**Ưu Điểm**:
- Rất chính xác, tính đến μs
- Tự động ghi thời điểm xung vào CCRx register
- Có thể đo tần số, chu kỳ, duty cycle
- Interrupt khi bắt xung

**Nhược Điểm**:
- Phức tạp hơn
- Cần xử lý interrupt

**Cách Sử Dụng**:
```c
// Cấu hình Input Capture
TIM_IC_InitTypeDef sConfigIC;
sConfigIC.ICPolarity = TIM_ICPOLARITY_RISING;  // Bắt cạnh lên
sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;  // Bắt mỗi xung
sConfigIC.ICFilter = 0;  // Không lọc

HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);
HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);  // Bắt đầu với interrupt

// Trong ISR hoặc callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        uint32_t pulse_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        // Xử lý giá trị pulse_time
    }
}
```

---

### **3. ENCODER MODE (Chế Độ Encoder) ⭐ Chuyên Dụng Nhất**

**Nguyên Lý**: Chế độ đặc biệt dành riêng cho encoder, tự động đếm xung A/B từ encoder.

**Hỗ Trợ**:
- ✅ TIM1, TIM2, TIM3, TIM4, TIM5, TIM8
- 3 chế độ encoder:
  - Mode 1: Đếm cạnh lên TI1
  - Mode 2: Đếm cạnh lên TI2
  - Mode 3: Đếm cạnh lên cả TI1 và TI2
- Tự động phát hiện chiều (A/B signals)

**Ưu Điểm**:
- **Tự động xác định chiều quay** (forward/backward)
- Rất chính xác, phần cứng xử lý
- Chính xác nhất cho encoder

**Nhược Điểm**:
- Cần 2 tín hiệu đầu vào (TI1 và TI2)
- Không phải Timer nào cũng support (nhưng TIM2, TIM5 hỗ trợ tốt)

**Cách Sử Dụng**:
```c
// Cấu hình Encoder Mode
TIM_Encoder_InitTypeDef sConfig;
sConfig.EncoderMode = TIM_ENCODERMODE_TI12;  // Đếm cạnh lên cả TI1 và TI2
sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
sConfig.IC1Filter = 0;
sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
sConfig.IC2Filter = 0;

HAL_TIM_Encoder_Init(&htim2, &sConfig);
HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

// Đọc giá trị đếm (tự động xác định chiều)
uint16_t encoder_count = __HAL_TIM_GET_COUNTER(&htim2);

// Xác định chiều quay
int16_t direction = (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2)) ? -1 : 1;
```

---

### **4. EXTERNAL CLOCK MODE (Chế Độ Clock Ngoài) ⭐ Nâng Cao**

**Nguyên Lý**: Sử dụng tín hiệu ngoài làm nguồn clock để tăng bộ đếm trực tiếp.

**Hỗ Trợ**:
- ✅ TIM1, TIM2, TIM3, TIM4, TIM5, TIM8
- External Clock Mode 1: Clock từ TI1 hoặc TI2
- External Clock Mode 2: Clock từ ETR (External Trigger)

**Ưu Điểm**:
- Bộ đếm tăng trực tiếp với mỗi xung ngoài
- Chính xác từng xung
- Không cần phần mềm xử lý

**Nhược Điểm**:
- Cấu hình phức tạp
- Cần hiểu rõ clock tree

**Cách Sử Dụng**:
```c
// Cấu hình Clock ngoài từ TI1
TIM_ClockConfigTypeDef sClockSourceConfig;
sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_TI1;  // Clock từ TI1
sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
sClockSourceConfig.ClockFilter = 0;

HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

// Khởi động đếm
HAL_TIM_Base_Start(&htim2);

// Bộ đếm tự động tăng với mỗi xung trên TI1
```

---

## 📊 So Sánh 4 Phương Pháp

| Tiêu Chí | Counter Mode | Input Capture | Encoder Mode | External Clock |
|----------|-------------|---------------|--------------|-----------------|
| **Độ Chính Xác** | Cao | Rất Cao | Rất Cao | Tuyệt Vời |
| **Dễ Sử Dụng** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ |
| **Tự Động Chiều** | ❌ | ❌ | ✅ | ❌ |
| **Tần Số Cao** | Có | Có | Có | Tuyệt Vời |
| **Tài Nguyên** | Thấp | Trung Bình | Trung Bình | Thấp |
| **Tính Năng** | Đếm cơ bản | Đo thời gian | Encoder chuyên dụng | Clock ngoài |

---

## 🏆 Khuyến Nghị cho Dự Án

Cho **điều khiển servo motor với encoder**:

### **Tốt Nhất: Encoder Mode**
```
Lý Do:
✅ Tự động xác định chiều quay (A/B signals)
✅ Chính xác nhất (phần cứng xử lý)
✅ Không cần phần mềm phức tạp
✅ Tần số cao (>100kHz)
```

### **Nếu không có 2 encoder signals: Counter Mode + Input Capture**
```
Lý Do:
✅ Hybrid: Counter Mode đếm xung, Input Capture bắt thời gian
✅ Xác định chiều bằng GPIO thay đổi
✅ Vẫn chính xác cao
```

---

## 🔧 Cấu Hình STM32F407 cho Pulse Counting

### **Timer Tốt Nhất cho STM32F407**

| Timer | Bit Width | Hỗ Trợ Encoder | Hỗ Trợ IC | Tốc Độ |
|-------|-----------|----------------|-----------|--------|
| **TIM2** | 32-bit | ✅ | ✅ | APB1 |
| **TIM3** | 16-bit | ✅ | ✅ | APB1 |
| **TIM4** | 16-bit | ❌ | ✅ | APB1 |
| **TIM5** | 32-bit | ✅ | ✅ | APB1 |
| **TIM9** | 16-bit | ❌ | ✅ | APB2 (2x tốc độ) |

**Khuyến nghị**: **TIM2 hoặc TIM5** (32-bit, hỗ trợ encoder)

---

## 💾 Ví Dụ Code Hoàn Chỉnh

### **Encoder Mode (Tốt nhất)**
```c
#include "stm32f4xx_hal.h"

TIM_HandleTypeDef htim2;

void Encoder_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF;  // 32-bit max
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim2);
    
    TIM_Encoder_InitTypeDef sConfig = {0};
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 10;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 10;
    
    HAL_TIM_Encoder_Init(&htim2, &sConfig);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
}

uint32_t Get_Encoder_Count(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}

int32_t Get_Encoder_Direction(void)
{
    // Nếu CNT giảm = quay lùi, CNT tăng = quay tiến
    return (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2)) ? -1 : 1;
}

void Motor_Move_Exact_Pulses(uint32_t pulses)
{
    uint32_t start_count = Get_Encoder_Count();
    uint32_t target_count = start_count + pulses;
    
    // Bật motor (PWM + Direction)
    HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_SET);  // Forward
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);  // 50% PWM
    
    // Đợi đến pulse count mục tiêu
    while (Get_Encoder_Count() < target_count) {
        // Đợi
        if (Get_Encoder_Count() >= target_count * 95 / 100) {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 250);  // Giảm speed 25%
        }
    }
    
    // Dừng motor
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
}
```

---

## 📍 Câu Hỏi Thường Gặp

**Q: Timer nào tốt nhất cho counting pulse?**
A: TIM2 hoặc TIM5 (32-bit, hỗ trợ encoder, APB1 clock)

**Q: Có thể đếm >65535 xung?**
A: Có! Dùng 32-bit Timer (TIM2, TIM5) hoặc sử dụng overflow interrupt

**Q: Làm sao để đảm bảo chính xác không mất xung?**
A: Dùng **Encoder Mode** (phần cứng xử lý) hoặc **Input Capture Mode** (có interrupt)

**Q: Tốc độ clock tối đa là bao nhiêu?**
A: APB1 = 42 MHz, APB2 = 84 MHz. Input Capture có thể xử lý >1 MHz

---

## ✅ Kết Luận

**Để điều khiển chính xác số xung trên STM32F407:**

🏆 **Đề xuất #1**: **ENCODER MODE** (nếu có 2 encoder signals)
- Tự động xác định chiều
- Chính xác nhất
- Code đơn giản

🥈 **Đề xuất #2**: **INPUT CAPTURE MODE** (nếu cần flexibility)
- Rất chính xác
- Có thể đo thời gian/tần số
- Linh hoạt hơn

🥉 **Đề xuất #3**: **COUNTER MODE** (đơn giản nhất)
- Dễ hiểu
- Đủ chính xác
- Tốn ít tài nguyên

