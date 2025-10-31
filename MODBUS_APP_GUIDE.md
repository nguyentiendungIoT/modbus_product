# HƯỚNG DẪN SỬ DỤNG MODBUS_APP.C

## 📌 MỤC ĐÍCH CỦA FILE NÀY

File `modbus_app.c` là **lớp ứng dụng Modbus** - nó đóng vai trò như một "người phiên dịch" giữa:
- **HMI (màn hình cảm ứng)** ← gửi lệnh qua Modbus
- **VXL (vi xử lý STM32)** ← điều khiển motor, đọc cảm biến

```
HMI → [Modbus Protocol] → modbus_app.c → [Motor Control] → Motor thực tế
```

---

## 🎯 CÁCH HOẠT ĐỘNG TỔNG QUÁT

### Bước 1: Khởi tạo (chỉ gọi 1 lần lúc boot)
```c
ModbusApp_Init();  // Trong hàm main(), sau khi init Modbus
```

**Công việc:**
- Xóa bộ nhớ teaching về 0
- Bật LED Xanh (hệ thống sẵn sàng)
- Tắt LED Đỏ

---

### Bước 2: Xử lý liên tục (gọi trong vòng lặp)
```c
while (1) {
    ModbusApp_Process();  // Kiểm tra lệnh từ HMI và thực thi
    HAL_Delay(10);        // Hoặc đặt trong timer interrupt
}
```

**Công việc:**
- Kiểm tra **các nút bấm** (coils) từ HMI
- Kiểm tra **dữ liệu** (registers) từ HMI
- Cập nhật LED trạng thái

---

## 📋 CÁC CHỨC NĂNG CHÍNH

### 1️⃣ ĐIỀU KHIỂN MOTOR THỦ CÔNG

#### Cách hoạt động:
HMI có 6 nút điều khiển motor (giống joystick):

| Nút trên HMI | Địa chỉ Modbus | Hành động |
|--------------|----------------|-----------|
| X ← (Trái)   | Coil 00008     | Motor X chạy trái |
| X → (Phải)   | Coil 00009     | Motor X chạy phải |
| Y ↑ (Vào)    | Coil 00010     | Motor Y chạy vào trong |
| Y ↓ (Ra)     | Coil 00011     | Motor Y chạy ra ngoài |
| Z ↑ (Lên)    | Coil 00012     | Motor Z chạy lên |
| Z ↓ (Xuống)  | Coil 00013     | Motor Z chạy xuống |

#### Ví dụ:
```
Người dùng GIỮ nút "X →" trên màn hình
  ↓
HMI gửi: Coil 00009 = 1 (BẬT)
  ↓
modbus_app.c nhận được → Gọi Move_x(Right, 100)
  ↓
Motor X chạy phải với tốc độ 100Hz
  ↓
Người dùng THÔI GIỮ nút
  ↓
HMI gửi: Coil 00009 = 0 (TẮT)
  ↓
modbus_app.c nhận được → Gọi Stop_x()
  ↓
Motor X dừng lại
```

---

### 2️⃣ DI CHUYỂN ĐẾN TỌA ĐỘ CỤ THỂ (SET)

#### Cách hoạt động:
1. Người dùng nhập tọa độ trên HMI:
   - X = 1500 → ghi vào register `400001`
   - Y = 2000 → ghi vào register `400002`
   - Z = 500  → ghi vào register `400003`

2. Người dùng nhấn nút **SET** (Coil 00014)

3. `modbus_app.c` tự động:
   - Tính khoảng cách cần di chuyển
   - Di chuyển tất cả 3 motor đến tọa độ đích

#### Code thực tế:
```c
// Đọc tọa độ đích từ HMI
uint16_t target_x = Holding_Registers_Database[REG_COORD_X];  // 400001
uint16_t target_y = Holding_Registers_Database[REG_COORD_Y];  // 400002
uint16_t target_z = Holding_Registers_Database[REG_COORD_Z];  // 400003

// Tính khoảng cách
int32_t delta_x = target_x - *Handler.Cur_x;  // VD: 1500 - 1000 = 500 (cần đi 500 bước)

// Di chuyển
if (delta_x > 0) {
    Move_x_2_target(500, Right);  // Đi 500 bước sang phải
}
```

---

### 3️⃣ TEACHING (LƯU VỊ TRÍ)

#### Mục đích:
Lưu 3 vị trí quan trọng của **Glass** và **Cover** để robot biết đâu là nơi cần pick/place.

#### Cách sử dụng:

**Bước 1: Di chuyển robot đến vị trí mong muốn** (thủ công)
```
Người dùng dùng các nút X/Y/Z để đưa robot đến:
  - Vị trí Glass số 1: (X=1000, Y=2000, Z=500)
```

**Bước 2: Nhấn nút Teaching**
```
Người dùng nhấn "Glass Pos1" (Coil 00025)
  ↓
modbus_app.c lưu tọa độ hiện tại vào bộ nhớ:
  TeachMem.Glass_Pos[0][0] = 1000;  // X
  TeachMem.Glass_Pos[0][1] = 2000;  // Y
  TeachMem.Glass_Pos[0][2] = 500;   // Z
  TeachMem.Glass_Valid[0] = 1;      // Đánh dấu đã lưu
  ↓
Nút trên HMI chuyển sang màu XANH (feedback)
```

**Bước 3: Lặp lại cho Pos2, Pos3**
```
Glass Pos2 → Coil 00026
Glass Pos3 → Coil 00027
```

**Xóa teaching:**
```
Nhấn nút "Glass Reset" (Coil 00028)
  ↓
modbus_app.c xóa tất cả 3 vị trí đã lưu
  ↓
Các nút Pos1/Pos2/Pos3 chuyển về màu ĐEN
```

---

### 4️⃣ CHỌN GÓC PHẦN TƯ (QUARTER)

#### Mục đích:
Màn hình Glass có 4 góc (Quarter 1, 2, 3, 4). Khi chọn góc khác nhau, robot sẽ pick từ vị trí khác nhau.

#### Cách hoạt động:
```
Người dùng nhấn "Quarter 2" (Coil 00018)
  ↓
modbus_app.c tự động:
  1. Reset màu tất cả ô Glass về XÁM
  2. Ghi flag 300 vào register 400100
  ↓
HMI nhận được flag → Vẽ lại màn hình Glass
```

---

### 5️⃣ XỬ LÝ KHAY CAMERA (25 Ô)

#### Sơ đồ khay Camera:
```
[1]  [2]  [3]  [4]  [5]
[6]  [7]  [8]  [9]  [10]
[11] [12] [13] [14] [15]
[16] [17] [18] [19] [20]
[21] [22] [23] [24] [25]
```

#### Cách hoạt động:
```
Người dùng click vào ô số 5 trên HMI
  ↓
HMI gửi: Register 400015 = 500 (màu XANH LÁ)
  ↓
modbus_app.c phát hiện ô 5 có màu xanh
  ↓
TODO: Thêm vào hàng đợi pick (chưa implement)
  ↓
Robot sẽ pick camera từ ô số 5
```

#### Đặt màu ô từ code:
```c
ModbusApp_SetCameraColor(5, COLOR_CAMERA_GREEN);  // Ô 5 → Xanh lá (có camera)
ModbusApp_SetCameraColor(5, COLOR_CAMERA_GRAY);   // Ô 5 → Xám (trống)
```

---

### 6️⃣ XỬ LÝ KHAY GLASS (196 Ô = 14x14)

#### Khái niệm:
Khay Glass là lưới 14x14 ô. Người dùng chọn **ô BẮT ĐẦU** (màu VÀNG), robot sẽ pick theo thứ tự từ ô đó.

#### Cách hoạt động:
```
Người dùng click vào ô số 50 (hàng 4, cột 8)
  ↓
HMI gửi: Register 400150 = 400 (màu VÀNG)
  ↓
modbus_app.c phát hiện ô 50 có màu vàng
  ↓
TODO: Bắt đầu pick từ ô 50 → 51 → 52 → ... (chưa implement)
  ↓
Sau khi pick xong ô 50 → Đổi màu về XÁM
```

#### Reset toàn bộ khay:
```
Người dùng nhấn "Reset Home" (Coil 00021)
  ↓
modbus_app.c ghi flag 300 vào register 400100
  ↓
Tất cả 196 ô đổi về màu XÁM
```

---

### 7️⃣ LED TRẠNG THÁI

| Trạng thái | LED Xanh | LED Đỏ | Ý nghĩa |
|------------|----------|--------|---------|
| Sẵn sàng   | BẬT ✅   | TẮT ❌ | Tất cả motor dừng |
| Đang chạy  | TẮT ❌   | TẮT ❌ | Motor đang di chuyển |
| Lỗi        | TẮT ❌   | BẬT ✅ | Có lỗi xảy ra |

#### Code kiểm tra:
```c
void ModbusApp_UpdateStatusLED(void)
{
    bool all_idle = (Motor_x.state == Stop && 
                     Motor_y.state == Stop && 
                     Motor_z.state == Stop);
    
    if (all_idle) {
        // BẬT LED XANH
        Coils_Database[2] |= (1 << (COIL_LED_GREEN - 16));
    }
}
```

---

## 🗺️ SƠ ĐỒ BỘ NHỚ MODBUS

### Coils (Nút bấm - Chỉ đọc 1 bit)
```
00001-00002: Chuyển tab (Home/Engineer)
00008-00013: Điều khiển motor (X/Y/Z Left/Right/In/Out/Up/Down)
00014-00015: SET/RESET tọa độ
00017-00020: Chọn góc phần tư (Quarter 1/2/3/4)
00021-00022: Reset Home, LED Xanh
00023:       LED Đỏ
00025-00027: Teaching Glass Pos1/2/3
00028:       Reset teaching Glass
00029-00031: Teaching Cover Pos1/2/3
00032:       Reset teaching Cover
```

### Holding Registers (Dữ liệu - 16-bit mỗi ô)
```
400001-400003: Tọa độ đích X/Y/Z
400010:        Ô Camera đang chọn (1-25)
400011-400035: Màu 25 ô Camera
400100:        Flag reset khay Glass
400101-400296: Màu 196 ô Glass
```

---

## 🚀 CÁCH TÍCH HỢP VÀO DỰ ÁN

### File `main.c`:
```c
#include "My_Lib/modbus_app.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    
    // Init peripherals
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_UART4_Init();
    
    // Init Modbus
    ModbusSlave_Init();
    
    // ✅ THÊM DÒNG NÀY
    ModbusApp_Init();
    
    while (1)
    {
        // ✅ THÊM DÒNG NÀY
        ModbusApp_Process();
        
        HAL_Delay(10);
    }
}
```

### File `RS232.c` (callback UART):
```c
#include "My_Lib/modbus_app.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == UART4)
    {
        ModbusProcess();
        
        // ✅ THÊM DÒNG NÀY (sau khi xử lý Modbus xong)
        ModbusApp_Process();
        
        HAL_UARTEx_ReceiveToIdle_DMA(&huart4, RxData, 512);
    }
}
```

---

## 🐛 XỬ LÝ LỖI

### Lỗi 1: Motor không chạy khi nhấn nút
**Nguyên nhân:** Con trỏ tọa độ chưa được link.

**Giải pháp:**
```c
// Trong main.c, sau khi init motor
Handler.Cur_x = &Motor_x.CurrentPos;
Handler.Cur_y = &Motor_y.CurrentPos;
Handler.Cur_z = &Motor_z.CurrentPos;
```

### Lỗi 2: Teaching không lưu được
**Nguyên nhân:** Kiểm tra con trỏ NULL.

**Kiểm tra:**
```c
if (Handler.Cur_x != NULL && Handler.Cur_y != NULL && Handler.Cur_z != NULL)
{
    // OK - Có thể lưu
}
else
{
    // LỖI - Chưa link con trỏ
}
```

### Lỗi 3: LED không đổi màu
**Nguyên nhân:** Hàm `ModbusApp_UpdateStatusLED()` không được gọi.

**Giải pháp:** Hàm này đã được gọi tự động trong `ModbusApp_Process()`.

---

## 📊 WORKFLOW THỰC TÊ

### Kịch bản 1: Pick 1 camera từ ô số 10
```
1. Người dùng click ô 10 trên HMI
2. HMI gửi: Register 400020 = 500 (màu XANH)
3. modbus_app.c phát hiện ô 10 có màu xanh
4. [TODO] Thêm vào queue: pick_camera_queue.push(10)
5. [TODO] Robot di chuyển đến ô 10
6. [TODO] Hạ Z xuống, bật vacuum, nâng Z lên
7. [TODO] Di chuyển đến vị trí place
8. Đổi màu ô 10 về XÁM (trống)
```

### Kịch bản 2: Pick glass từ góc phần tư 2
```
1. Người dùng nhấn "Quarter 2"
2. modbus_app.c reset màu 196 ô về XÁM
3. Người dùng click ô 50 (chọn điểm bắt đầu)
4. HMI gửi: Register 400150 = 400 (màu VÀNG)
5. modbus_app.c phát hiện → Bắt đầu pick từ ô 50
6. [TODO] Pick glass theo thứ tự: 50→51→52→...
7. Sau khi pick xong 1 ô → Đổi màu về XÁM
```

---

## 💡 GỢI Ý PHÁT TRIỂN TIẾP

### 1. Thêm Queue Pick
```c
typedef struct {
    uint8_t cell_index;  // Số ô (1-25 hoặc 1-196)
    uint8_t type;        // 0=Camera, 1=Glass
} PickTask_t;

PickTask_t pick_queue[100];
uint8_t queue_head = 0;
uint8_t queue_tail = 0;
```

### 2. State Machine cho Robot
```c
typedef enum {
    STATE_IDLE,
    STATE_MOVING_TO_PICK,
    STATE_PICKING,
    STATE_MOVING_TO_PLACE,
    STATE_PLACING,
    STATE_ERROR
} RobotState_t;
```

### 3. Lưu Teaching vào Flash
```c
// Sau khi teaching xong
Flash_Write(FLASH_ADDR_TEACH, &TeachMem, sizeof(TeachMem));

// Lúc boot
Flash_Read(FLASH_ADDR_TEACH, &TeachMem, sizeof(TeachMem));
```

---

## 📞 HỖ TRỢ

Nếu còn thắc mắc, hãy kiểm tra:
1. **MOTOR_DOCUMENTATION.md** - Giải thích motor.c
2. **modbus_app.h** - Định nghĩa tất cả địa chỉ Modbus
3. **main.c** - Xem cách tích hợp vào main loop

**Chúc bạn thành công! 🎉**
