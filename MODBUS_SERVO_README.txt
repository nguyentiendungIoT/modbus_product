==============================================
  ĐIỀU KHIỂN SERVO QUA MODBUS - HƯỚNG DẪN
==============================================

✅ ĐÃ TÍCH HỢP VÀO main.c - KHÔNG CẦN FILE THÊM

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📋 MODBUS MAPPING

Coils (HMI → STM32):
  Coil 5  (addr 4) →  X Left   (X-)
  Coil 6  (addr 5) →  X Right  (X+)
  Coil 7  (addr 6) →  Y Up     (Y+)
  Coil 8  (addr 7) →  Y Down   (Y-)
  Coil 9  (addr 8) →  Z Up     (Z+)
  Coil 10 (addr 9) →  Z Down   (Z-)

Modbus UART: USART2, 115200, 8N1, Slave ID = 7

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

⚙️ PHẦN CỨNG

PWM (10kHz cố định):
  X: TIM1_CH1 (PA8)  - DIR: PC6
  Y: TIM8_CH1 (PC6)  - DIR: PC7
  Z: TIM3_CH3 (PC8)  - DIR: PC9

Counter (đếm pulse):
  TIM2 → đếm X
  TIM5 → đếm Y
  TIM9 → đếm Z

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🎯 CÁCH HOẠT ĐỘNG

1. HMI gửi Write Single Coil (FC 05):
   - Địa chỉ: 5 (X Right)
   - Value: 0xFF00 (ON)

2. STM32 nhận trong HAL_UARTEx_RxEventCallback()
   - Xử lý Modbus
   - Đọc Coil state
   - Set DIR pin (PC6 = HIGH)
   - Set ARR = 10 (10kHz)
   - Start PWM TIM1

3. HMI thả nút:
   - Value: 0x0000 (OFF)
   - Stop PWM

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🧪 TEST VỚI MODBUS POLL

1. Connection: COM port, 115200, 8N1
2. Slave ID: 7
3. Test Write Single Coil (05):
   Address: 5
   Value: 1
   → X servo di chuyển phải

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🛠️ ĐIỀU CHỈNH

Thay đổi tốc độ PWM:
  #define PWM_FREQ_10KHZ  10  → Đổi thành số khác

Thay đổi DIR pin:
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, ...)
  → Đổi thành GPIO và Pin phù hợp

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📝 LOGIC XỬ LÝ

Ví dụ X axis:
  - Nếu X_NEG = 1 và X_POS = 0 → Di chuyển trái
  - Nếu X_POS = 1 và X_NEG = 0 → Di chuyển phải
  - Nếu cả 2 = 1 hoặc cả 2 = 0 → Dừng

Tương tự cho Y, Z.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✔️ ĐÃ THỰC HIỆN

✓ Thêm includes (modbusSlave.h, string.h)
✓ Thêm RxData[256], TxData[256] buffers
✓ Define 6 coils (COIL_X_NEG ... COIL_Z_NEG)
✓ Macro GET_COIL(addr) để đọc bit
✓ Tạo các hàm điều khiển motor riêng biệt:
  - Motor_X_Move(direction, speed)
  - Motor_Y_Move(direction, speed)
  - Motor_Z_Move(direction, speed)
  - Motor_X/Y/Z_Stop()
  - Motor_All_Stop()
✓ HAL_UARTEx_RxEventCallback() xử lý Modbus + gọi hàm motor
✓ Khởi tạo UART Modbus trong main()
✓ Main loop chỉ delay 10ms

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

📦 CẤU TRÚC CODE - TÁCH RIÊNG ĐIỀU KHIỂN MOTOR

Các hàm motor (trong main.c):

1. Motor_X_Move(direction, speed)
   - direction: 0=trái, 1=phải
   - speed: ARR value (10 = 10kHz)
   - Tự động set DIR pin + ARR + start PWM

2. Motor_Y_Move(direction, speed)
   - direction: 0=xuống, 1=lên
   
3. Motor_Z_Move(direction, speed)
   - direction: 0=xuống, 1=lên

4. Motor_X/Y/Z_Stop()
   - Dừng motor tương ứng

5. Motor_All_Stop()
   - Dừng tất cả (Emergency)

VÍ DỤ SỬ DỤNG:
  // Di chuyển X phải với 10kHz
  Motor_X_Move(1, 10);
  
  // Di chuyển Y lên với tốc độ khác
  Motor_Y_Move(1, 20);  // ARR=20 (chậm hơn)
  
  // Dừng tất cả
  Motor_All_Stop();

LỢI ÍCH:
  ✓ Dễ mở rộng thêm trường hợp điều khiển khác
  ✓ Có thể thay đổi tốc độ linh hoạt
  ✓ Code gọn, dễ đọc
  ✓ Tái sử dụng được (gọi từ nhiều nơi)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🚀 BUILD & TEST

1. Clean → Build project
2. Flash to STM32
3. Test với ModBus Poll hoặc HMI

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

⚠️ LƯU Ý

- Tốc độ PWM cố định 10kHz
- DIR pin: HIGH = +, LOW = -
- Không dùng struct vì logic đơn giản, trực tiếp if-else
- Xử lý ngay trong UART callback để đáp ứng nhanh

==============================================
