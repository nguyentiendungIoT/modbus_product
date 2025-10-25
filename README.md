# STM32F407 Modbus Servo Control

## Giới thiệu
Dự án này là firmware cho STM32F407VET6, điều khiển 3 trục servo qua Modbus RTU (UART). Mỗi trục có 2 coil điều khiển chiều, hỗ trợ chế độ liên tục và đếm xung. Code đã được refactor thành các module riêng biệt: motor_control, modbus_handler, main.

## Phần cứng
- MCU: STM32F407VET6
- UART: USART2 (Modbus RTU, slave ID=7, 115200)
- PWM: TIM1_CH1 (X), TIM8_CH1 (Y), TIM3_CH3 (Z) @ 10kHz
- Counter: TIM2 (X), TIM5 (Y), TIM9 (Z) - đếm xung ngoài
- GPIO: DIR1_CHx, DIR2_CHx (điều khiển chiều)

## Build
- Yêu cầu: Windows, ARM GCC toolchain, CubeIDE hoặc build.bat
- Build nhanh:
  ```powershell
  .\build.bat
  ```
- File output: `Debug/test.elf`

## Flash firmware
- Dùng CubeIDE hoặc:
  ```powershell
  .\flash.bat
  ```
- Kết nối ST-Link/J-Link với board

## Sử dụng Modbus
- Modbus RTU slave, ID=7, baud 115200
- Coil mapping:
  | Coil      | Địa chỉ | Ý nghĩa         |
  |-----------|---------|-----------------|
  | COIL_X_NEG| 3       | X trái          |
  | COIL_X_POS| 4       | X phải          |
  | COIL_Y_POS| 5       | Y lên           |
  | COIL_Y_NEG| 6       | Y xuống         |
  | COIL_Z_POS| 7       | Z lên           |
  | COIL_Z_NEG| 8       | Z xuống         |
- Để chạy liên tục: set coil tương ứng, pulses=0
- Để chạy theo số xung: ghi pulses vào register, set coil

## Test chức năng
- Gửi lệnh Modbus từ HMI/PC/PLC
- Kiểm tra motor quay đúng chiều, đúng số xung
- Xem debug qua UART hoặc LED

## Đóng góp
- Mọi ý kiến, bug, hoặc yêu cầu mở rộng vui lòng tạo issue hoặc pull request tại repo này.

---
**Tác giả:** nguyentiendungIoT
**Repo:** https://github.com/nguyentiendungIoT/modbus_product
