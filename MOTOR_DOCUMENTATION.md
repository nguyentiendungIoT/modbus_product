# Tài liệu Motor Control System (motor.c)

**Ngày:** October 31, 2025  
**File:** `Core/Src/My_Lib/motor.c`  
**Mục đích:** Điều khiển 3 động cơ (X, Y, Z) qua STM32F407 Timers với 2 chế độ: chạy bằng tay (manual) và chạy tự động tới mục tiêu.

---

## 1. Tổng quan kiến trúc

### Cấu trúc dữ liệu chính

```c
// Motor state
Motor_t Motor_x, Motor_y, Motor_z {
    .dir      -> hướng (Left/Right, In/Out, Up/Down)
    .state    -> trạng thái (Stop, Run_by_Hand, Run_Auto)
}

// Handler (bộ điều khiển vị trí)
Handler_t Handler {
    .Cur_x, .Cur_y, .Cur_z        -> con trỏ vị trí hiện tại
    .Old_Position_x/y/z           -> vị trí cũ (lưu trước đó)
    .Taget_x, .Taget_y, .Taget_z  -> vị trí mục tiêu (auto mode)
}

// Timers sử dụng
htim1, htim2 (X axis)
htim8, htim5 (Y axis)
htim3, htim9 (Z axis)
```

### Các Timer và vai trò

| Trục | Timer Tần số (PWM) | Timer Đếm (Slave) | Mục đích |
|------|-------------------|-------------------|---------|
| X    | TIM1 (2 MHz)      | TIM2              | Sinh xung PWM & nhận tín hiệu từ TIM1 (TRGO) |
| Y    | TIM8 (2 MHz)      | TIM5              | Sinh xung PWM & nhận tín hiệu từ TIM8 (TRGO) |
| Z    | TIM3 (1 MHz)      | TIM9              | Sinh xung PWM & nhận tín hiệu từ TIM3 (TRGO) |

**Nguyên lý:** Timer PWM phát xung điều khiển tốc độ → Timer Slave nhận xung để đếm số bước → Khi đếm đủ số pulse mục tiêu → Ngắt → Gọi Stop_x/y/z.

---

## 2. Luồng khởi tạo (Initialization Flow)

### 2.1 Khởi tạo một trục (ví dụ X)

```
Motor_x_Init()
    ├─→ MX_TIM1_Init()  
    │   ├─ Cấu hình TIM1 PWM (Prescaler=83, Period=99)
    │   ├─ Thiết lập TRGO = OC1REF (phát xung điều khiển)
    │   └─ Kích hoạt HAL_TIM_PWM_Init
    │
    ├─→ MX_TIM2_Init()
    │   ├─ Cấu hình TIM2 Slave (Prescaler=0, Period=19)
    │   ├─ Chế độ: EXTERNAL1 trigger từ ITR0 (TIM1 TRGO)
    │   └─ Kích hoạt HAL_TIM_Base_Init
    │
    └─→ HAL_TIM_Base_Start_IT(&htim2)
        ├─ Bật bộ đếm TIM2 (sẵn sàng nhận xung từ TIM1)
        ├─ Bật ngắt UPDATE cho TIM2
        └─ NVIC sẽ gọi TIM2_IRQHandler khi TIM2 tràn
```

**Kết quả:** Trục X sẵn sàng, chờ lệnh di chuyển.

---

## 3. Hai chế độ di chuyển

### Chế độ 1: Chạy bằng tay (Manual / Run_by_Hand)

```
Move_x(Left, 100Hz)
    ├─ Motor_x.dir = Left
    ├─ Motor_x.state = Run_by_Hand
    ├─ htim2.Instance->ARR = 0xFFFFFFFFu  // ARR rất lớn → không tự động tràn
    ├─ htim1.Instance->ARR = (2000000 / 100) - 1 = 19999
    │   └─ Tính tần số PWM: 2MHz / (19999+1) = 100 Hz (tốc độ motor)
    │
    └─→ HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1)
        ├─ TIM1 bắt đầu phát xung PWM 100 Hz
        ├─ TIM2 nhận xung từ TIM1 TRGO và bắt đầu đếm
        └─ Motor X chạy liên tục với tốc độ 100 Hz
        
Đằng sau scenes:
    • TIM1 tạo sóng PWM, đồng thời phát TRGO (trigger)
    • TIM2 (slave) nhận TRGO, CNT tăng 1 cho mỗi pulse nhận được
    • TIM2 CNT được ghi lại trong lúc chạy (dùng sau trong Stop_x)

Dừng lại:
    Stop_x()
        ├─ HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1)  // TIM1 dừng phát
        ├─ Tính vị trí hiện tại dựa trên hướng + CNT của TIM2:
        │   *Handler.Cur_x = Handler.Old_Position_x ± htim2.Instance->CNT
        ├─ Motor_x.state = Stop
        ├─ Lưu Old_Position_x = *Handler.Cur_x (cho lần chạy tiếp)
        └─ Reset CNT: htim2.Instance->CNT = 0
```

**Sơ đồ luồng Manual:**
```
┌─────────────────────────────────────────────┐
│  Move_x(dir, freq)                          │
│  - Set ARR rất lớn (không auto-stop)        │
│  - Khởi động TIM1 PWM                       │
└──────────────┬──────────────────────────────┘
               │
               ▼
        ┌────────────────┐
        │ Motor chạy     │◄── Người dùng gọi Stop_x()
        │ liên tục       │    khi muốn dừng
        │ CNT tăng 1/pulse
        └────────────────┘
               │
               ▼
        ┌────────────────┐
        │ Stop_x()       │
        │ Cập nhật vị trí│
        │ Motor dừng     │
        └────────────────┘
```

---

### Chế độ 2: Chạy tới mục tiêu (Automatic / Run_Auto)

```
Move_x_2_target(100 pulse, Left)
    ├─ Motor_x.dir = Left
    ├─ Motor_x.state = Run_Auto
    ├─ htim2.Instance->ARR = 100 - 1 = 99   // ⭐ Key point: ARR = số pulse mục tiêu
    │   └─ TIM2 sẽ tạo UPDATE interrupt khi CNT từ 0→99
    │
    ├─ htim1.Instance->ARR = (2000000 / F_high) - 1
    │   └─ F_high là hằng (tốc độ cố định cao)
    │
    └─→ HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1)
        ├─ TIM1 phát xung PWM
        ├─ TIM2 nhận xung từ TRGO, CNT = 0, 1, 2, ..., 99
        └─ Khi CNT đạt 99 (= ARR) → TIM2 tạo UPDATE flag
        
Xử lý ngắt tự động:
    TIM2_IRQHandler() được gọi khi TIM2 tràn (CNT vượt ARR)
        ├─ Kiểm tra SR: UPDATE flag = 1 ? 
        ├─ Kiểm tra DIER: UPDATE interrupt enabled ?
        ├─ Nếu YES → Stop_x() được gọi ngay lập tức
        ├─ Motor dừng, vị trí cập nhật
        └─ HAL_TIM_IRQHandler(&htim2) xóa cờ
```

**Sơ đồ luồng Auto:**
```
┌──────────────────────────────────┐
│ Move_x_2_target(100, Left)       │
│ ARR = 99 (trigger ngắt)          │
│ Khởi động TIM1 & TIM2            │
└────────────────┬─────────────────┘
                 │
                 ▼
       ┌─────────────────────┐
       │ TIM1 phát xung      │
       │ TIM2 đếm xung       │
       │ CNT: 0→1→2→...→99   │
       └──────────┬──────────┘
                  │
                  ▼ (CNT vượt ARR)
       ┌─────────────────────┐
       │ TIM2 UPDATE flag    │
       │ NVIC gọi TIM2_IRQn  │
       └──────────┬──────────┘
                  │
                  ▼
       ┌─────────────────────┐
       │ TIM2_IRQHandler     │
       │ ├─ kiểm tra flag    │
       │ ├─ Stop_x() (auto)  │
       │ └─ xóa cờ           │
       └─────────────────────┘
```

---

## 4. Hàm quốc bộ (Core Functions)

### 4.1 Hàm khởi tạo

| Hàm | Mục đích | Ghi chú |
|-----|---------|--------|
| `Motor_x_Init()` | Cấu hình TIM1, TIM2, bật ngắt TIM2 | Chạy 1 lần ở startup |
| `Motor_y_Init()` | Cấu hình TIM8, TIM5, bật ngắt TIM5 | Chạy 1 lần ở startup |
| `Motor_z_Init()` | Cấu hình TIM3, TIM9, bật ngắt TIM9 | Chạy 1 lần ở startup |

### 4.2 Hàm di chuyển (Movement)

| Hàm | Tham số | Chế độ | Khi nào dừng |
|-----|--------|-------|------------|
| `Move_x(dir, freq)` | hướng + tần số | By-Hand | Gọi `Stop_x()` thủ công |
| `Move_x_2_target(pulse, dir)` | số pulse + hướng | Auto | Tự động khi đạt pulse (interrupt) |
| `Move_y(dir, freq)` | hướng + tần số | By-Hand | Gọi `Stop_y()` thủ công |
| `Move_y_2_target(pulse, dir)` | số pulse + hướng | Auto | Tự động khi đạt pulse (interrupt) |
| `Move_z(dir, freq)` | hướng + tần số | By-Hand | Gọi `Stop_z()` thủ công |
| `Move_z_2_target(pulse, dir)` | số pulse + hướng | Auto | Tự động khi đạt pulse (interrupt) |

### 4.3 Hàm dừng (Stop)

| Hàm | Tác vụ | Chi tiết |
|-----|--------|---------|
| `Stop_x()` | Dừng TIM1 PWM, cập nhật vị trí, reset CNT | Gọi từ `TIM2_IRQHandler` (auto) hoặc thủ công |
| `Stop_y()` | Dừng TIM8 PWM, cập nhật vị trí, reset CNT | Gọi từ `TIM5_IRQHandler` (auto) hoặc thủ công |
| `Stop_z()` | Dừng TIM3 PWM, cập nhật vị trí, reset CNT | Gọi từ `TIM1_BRK_TIM9_IRQHandler` (auto) hoặc thủ công |

### 4.4 Hàm hỗ trợ

| Hàm | Mục đích |
|-----|---------|
| `Move_Handle_2_point(Item)` | Tính delta từ vị trí hiện tại đến mục tiêu, gọi `Move_x_2_target` + `Move_y_2_target` |
| `Set_Frequency_Motor(f, f_max)` | Đặt tần số PWM cho tất cả timer (động) |
| `Tim2_Init()` / `Tim5_Init()` / `Tim9_Init()` | Khởi tạo lại timer nếu cần |

---

## 5. Luồng ngắt (Interrupt Flow)

### 5.1 TIM2_IRQHandler (Trục X - Auto Mode)

```
Kích phát: Khi TIM2 CNT tràn (CNT ≥ ARR)

TIM2_IRQHandler()
    ├─ Kiểm tra: (SR & TIM_FLAG_UPDATE) == TIM_FLAG_UPDATE ?
    ├─ Kiểm tra: (DIER & TIM_IT_UPDATE) == TIM_IT_UPDATE ?
    ├─ Nếu YES → Stop_x() được gọi
    │   ├─ HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1)
    │   ├─ if (Motor_x.state == Run_by_Hand)
    │   │   └─ *Handler.Cur_x = Old_Position_x ± htim2->CNT (tính dựa trên hướng)
    │   ├─ else if (Motor_x.state == Run_Auto)
    │   │   └─ *Handler.Cur_x = Handler.Taget_x (gán vị trí mục tiêu)
    │   ├─ Motor_x.state = Stop
    │   ├─ Old_Position_x = *Handler.Cur_x
    │   └─ htim2->CNT = 0
    │
    └─ HAL_TIM_IRQHandler(&htim2)
        ├─ Xóa cờ UPDATE
        └─ (Tuỳ cấu hình) Gọi HAL_TIM_PeriodElapsedCallback nếu có
```

### 5.2 TIM5_IRQHandler (Trục Y - Auto Mode)

Tương tự TIM2_IRQHandler, nhưng gọi `Stop_y()`.

### 5.3 TIM1_BRK_TIM9_IRQHandler (Trục Z - Auto Mode)

```
Kích phát: Khi TIM9 CNT tràn hoặc TIM1 break event

TIM1_BRK_TIM9_IRQHandler()
    ├─ Kiểm tra: (htim9->SR & TIM_FLAG_UPDATE) == TIM_FLAG_UPDATE ?
    ├─ Kiểm tra: (htim9->DIER & TIM_IT_UPDATE) == TIM_IT_UPDATE ?
    ├─ Nếu YES → Stop_z() được gọi (logic tương tự Stop_x)
    │
    └─ HAL_TIM_IRQHandler(&htim9)
        ├─ Xóa cờ UPDATE
        └─ (Tuỳ cấu hình) Gọi HAL_TIM_PeriodElapsedCallback
```

---

## 6. Ví dụ thực tế: Chuyển động đầy đủ

### Kịch bản: Di chuyển trục X tới 100 pulse sang trái, rồi dừng

```c
// Bước 1: Khởi tạo (lần đầu startup)
Motor_x_Init();  
// Kết quả: TIM1, TIM2 sẵn sàng, ngắt TIM2 bật

// Bước 2: Bắt đầu di chuyển tới 100 pulse
Move_x_2_target(100, Left);
// Kết quả: 
//   - Motor_x.state = Run_Auto
//   - htim2.ARR = 99 (sẽ tạo ngắt khi CNT=99)
//   - htim1 bắt đầu phát xung PWM tần số cao (F_high)
//   - TIM2 bắt đầu đếm xung từ TIM1

// Bước 3: Chờ (hệ thống sẽ tự động kích hoạt)
// Dòng thời gian:
// T0: TIM1 phát xung 0, TIM2 CNT=0
// T1: TIM1 phát xung 1, TIM2 CNT=1
// ...
// T99: TIM1 phát xung 99, TIM2 CNT=99 → SR.UPDATE = 1 → NVIC gọi TIM2_IRQHandler

// Bước 4: Ngắt tự động xảy ra
// TIM2_IRQHandler() thực thi:
//   → Stop_x() được gọi
//   → TIM1 PWM dừng
//   → *Handler.Cur_x = Handler.Taget_x = Old_Position_x + 100 (trừ vì Left)
//   → Motor_x.state = Stop
//   → CNT = 0

// ✓ Motor X dừng lại, vị trí được cập nhật tự động
```

---

## 7. Biểu đồ tương tác toàn diện

```
┌─────────────────────────────────────────────────────────────────────┐
│                    MOTOR CONTROL SYSTEM                             │
└─────────────────────────────────────────────────────────────────────┘

INITIALIZATION PHASE:
┌──────────────────┐
│  Motor_x_Init()  │ (Hoặc y, z)
│  Motor_y_Init()  │
│  Motor_z_Init()  │
└────────┬─────────┘
         │
         ├─→ MX_TIM1_Init() / MX_TIM8_Init() / MX_TIM3_Init()
         │   (Cấu hình PWM timers)
         │
         ├─→ MX_TIM2_Init() / MX_TIM5_Init() / MX_TIM9_Init()
         │   (Cấu hình Slave timers)
         │
         └─→ HAL_TIM_Base_Start_IT()
             (Bật ngắt cho slave timers)

                    ↓

MOVEMENT PHASE:
┌─────────────────────┐         ┌──────────────────────┐
│  Move_x(dir, freq)  │         │ Move_x_2_target(...) │
│  (Manual mode)      │         │ (Auto mode)          │
└──────────┬──────────┘         └──────────┬───────────┘
           │                               │
           ├─ Set Motor_x.state            ├─ Set Motor_x.state
           │  = Run_by_Hand                │  = Run_Auto
           │                               │
           ├─ ARR(TIM2) = 0xFFFFFFFFu      ├─ ARR(TIM2) = pulse-1
           │  (không auto-stop)            │  (sẽ trigger ngắt)
           │                               │
           ├─ ARR(TIM1) = freq formula    ├─ ARR(TIM1) = F_high formula
           │                               │
           └─ HAL_TIM_PWM_Start            └─ HAL_TIM_PWM_Start
             (TIM1 phát xung)                (TIM1 phát xung)
                   │                               │
                   ▼                               ▼
          Motor chạy liên tục         Motor chạy đến đủ pulse
          CNT tăng liên tục           CNT: 0→1→...→(pulse-1)
          
          (Người dùng phải gọi        (Tự động gọi Stop_x()
           Stop_x() để dừng)          khi CNT ≥ ARR)

                    ↓

INTERRUPT PHASE (TIM2_IRQHandler cho trục X):
┌──────────────────────────┐
│ TIM2_IRQHandler()        │
│ (Kích phát: CNT tràn)    │
│                          │
│ 1. Kiểm tra SR flag      │
│ 2. Kiểm tra DIER bit     │
│ 3. Gọi Stop_x()          │
│    ├─ Stop PWM (TIM1)    │
│    ├─ Cập nhật vị trí    │
│    ├─ Set state=Stop     │
│    └─ Reset CNT=0        │
│                          │
│ 4. HAL_TIM_IRQHandler()  │
│    └─ Xóa cờ, cleanup    │
└──────────────────────────┘

                    ↓

RESULT: Motor dừng, vị trí được cập nhật
```

---

## 8. Bảng trạng thái Motor (State Machine)

```
Motor_x.state:

    ┌─────────┐
    │  STOP   │ ◄─── Khởi tạo (Ban đầu)
    └────┬────┘
         │
         │ Move_x() hoặc Move_x_2_target()
         ▼
    ┌──────────────┐
    │ Run_by_Hand  │ (chạy liên tục, cần Stop_x() thủ công)
    └──────┬───────┘
           │
           │ Stop_x()
           │
    ┌──────────────┐
    │ Run_Auto     │ (chạy tới pulse cố định, tự dừng khi interrupt)
    └──────┬───────┘
           │
           │ Ngắt TIM2 (TIM2_IRQHandler → Stop_x())
           │
           ▼
    ┌─────────┐
    │  STOP   │ ✓ (Hoàn thành)
    └─────────┘
```

---

## 9. Các rủi ro / Lưu ý quan trọng

### ⚠️ Rủi ro 1: Con trỏ NULL
```c
// ❌ HIỆN TẠI (nguy hiểm):
Handler_t Handler = {
    .Cur_x = 0,  // ← Con trỏ NULL!
    .Cur_y = 0,
    .Cur_z = 0
};

// Sau đó trong Stop_x():
*Handler.Cur_x = ...;  // ← CRASH! Dereference NULL

// ✅ CẢI TIẾN:
if (Handler.Cur_x != NULL) {
    *Handler.Cur_x = ...;
}
```

### ⚠️ Rủi ro 2: ARR = -1 không rõ ràng
```c
// ❌ HIỆN TẠI:
htim2.Instance->ARR = -1;  // ← Ý gì? unsigned -1 = 0xFFFFFFFF

// ✅ CẢI TIẾN:
htim2.Instance->ARR = 0xFFFFFFFFu;  // Rõ ràng, an toàn
```

### ⚠️ Rủi ro 3: Flag không được xóa
```c
// ❌ HIỆN TẠI (bị comment):
// __HAL_TIM_CLEAR_FLAG(&htim9, TIM_FLAG_UPDATE);

// ✅ CẢI TIẾN: Uncomment để an toàn
__HAL_TIM_CLEAR_FLAG(&htim9, TIM_FLAG_UPDATE);
Stop_z();
```

---

## 10. Cheat Sheet - Cách sử dụng nhanh

### Khởi tạo hệ thống
```c
Motor_x_Init();
Motor_y_Init();
Motor_z_Init();
```

### Chạy bằng tay X sang trái với tốc độ 50 Hz
```c
Move_x(Left, 50);
// ... motor chạy ...
Stop_x();  // Dừng thủ công
```

### Chạy X tới 200 pulse sang phải (tự động)
```c
Handler.Taget_x = current_pos + 200;  // Đặt mục tiêu
Move_x_2_target(200, Right);
// ... motor chạy, TIM2 đếm ...
// Khi đạt 200 pulse → TIM2_IRQHandler gọi Stop_x() → dừng tự động
```

### Đặt tốc độ động
```c
Set_Frequency_Motor(75, 100);  // f=75 Hz, f_max=100 Hz
```

---

## 11. Sơ đồ Timing (Minh họa PWM & Pulse)

```
Move_x_2_target(5, Left):

TIM1 PWM Output (2 MHz / F_high):
───┐       ┌───┐       ┌───┐       ┌───┐       ┌───┐
   └───┬───┘   └───┬───┘   └───┬───┘   └───┬───┘   └───
       │           │           │           │           
TIM2 (Slave) - nhận pulse từ TIM1 TRGO:
  CNT: 0 → 1 → 2 → 3 → 4 → 5 (= ARR) → OVERFLOW → UPDATE flag
                                  ↓
                          TIM2_IRQHandler()
                                  ↓
                            Stop_x() được gọi
                                  ↓
                          Motor X dừng ✓
```

---

## 12. Tóm tắt luồng chính

| Giai đoạn | Hành động | Kết quả |
|-----------|-----------|--------|
| **Init** | Gọi Motor_x_Init() | Timer sẵn sàng, ngắt bật |
| **Move (Manual)** | Move_x(dir, freq) | Motor chạy liên tục |
| **Move (Auto)** | Move_x_2_target(pulse, dir) | Motor chạy tới pulse cố định |
| **Counting** | TIM2 đếm xung từ TIM1 | CNT tăng, phát UPDATE flag khi full |
| **Interrupt** | TIM2_IRQHandler() | Stop_x() gọi, motor dừng, vị trí cập nhật |
| **Idle** | Motor dừng | Chờ lệnh mới từ Move_x() hoặc Move_x_2_target() |

---

**Tạo bởi:** Documentation Generator  
**Mục đích:** Giúp hiểu rõ luồng hoạt động motor.c  
**Lần cập nhật cuối:** 2025-10-31

