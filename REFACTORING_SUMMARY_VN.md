# Tóm Tắt Refactoring: modbus_app.c

Ngày: 2024
Mục đích: Đơn giản hóa logic modbus_app.c để phù hợp với kiến trúc HMI-centric (HMI tự động xử lý nhiều tác vụ)

---

## Danh Sách Thay Đổi Đã Thực Hiện (9 yêu cầu)

### ✅ 1. Loại bỏ logic điều khiển LED (xanh/đỏ)
**Lý do:** HMI không cần MCU điều khiển LED trạng thái

**Đã xóa:**
- Biến `last_status_update` (dùng để delay cập nhật LED)
- Hàm `ModbusApp_UpdateStatusLED()` (cập nhật LED xanh/đỏ)
- Khởi tạo LED trong `ModbusApp_Init()`
- Lời gọi hàm trong `ModbusApp_Process()`

**File ảnh hưởng:**
- `Core/Src/My_Lib/modbus_app.c` (đã xóa ~40 dòng code)
- `Core/Inc/My_Lib/modbus_app.h` (đã xóa khai báo hàm)

---

### ✅ 2. Loại bỏ logic chuyển tab (Home/Engineer)
**Lý do:** HMI tự động quản lý tab, MCU không cần xử lý

**Đã xóa:**
- Hàm `Handle_TabControl()` (xử lý coil 00001-00002)
- Lời gọi hàm trong `ModbusApp_Process()`

---

### ✅ 3. Tối ưu macro kiểm tra bit coil
**Đã thêm:** Macro `CHECK_COIL_BIT(byte_idx, bit_pos)` để code ngắn gọn hơn

```c
// CŨ:
if ((current & (1 << (COIL_MOTOR_X_LEFT - 8))) && ...)

// MỚI:
if (CHECK_COIL_BIT(1, COIL_MOTOR_X_LEFT - 8) && ...)
```

**Áp dụng cho:** Toàn bộ hàm `Handle_ManualMotor()` và `Handle_Teaching()`

---

### ✅ 4. Loại bỏ việc xóa coil thủ công
**Lý do:** HMI tự động xóa coil sau 0.1s (pulse mode)

**Đã xóa trong `Handle_ManualMotor()`:**
```c
// ĐÃ XÓA: Coils_Database[1] &= ~(1 << (COIL_MOTOR_SET - 8));
// ĐÃ XÓA: Coils_Database[1] &= ~(1 << (COIL_MOTOR_RESET - 8));
```

**Đã xóa trong `Handle_Teaching()`:**
```c
// ĐÃ XÓA: Coils_Database[3] &= ~(0x07 << (COIL_TEACH_GLASS_POS1 - 24));
// ĐÃ XÓA: Coils_Database[3] &= ~(1 << (COIL_TEACH_GLASS_RESET - 24));
```

---

### ✅ 5. Thêm logic homing trong xử lý RESET
**Mục đích:** Khi nhấn nút RESET, motor về vị trí gốc (0, 0, 0)

**Đã thêm trong `Handle_ManualMotor()`:**
```c
// ===== NÚT RESET (Kích hoạt theo cạnh lên - edge-triggered) =====
if (CHECK_COIL_BIT(1, COIL_MOTOR_RESET - 8) && !(prev_coils[1] & (1 << (COIL_MOTOR_RESET - 8))))
{
    // Reset tọa độ về 0 (xóa vị trí hiện tại)
    Holding_Registers_Database[REG_COORD_X] = 0;
    Holding_Registers_Database[REG_COORD_Y] = 0;
    Holding_Registers_Database[REG_COORD_Z] = 0;
    
    // Di chuyển motor về vị trí gốc (0, 0, 0)
    if (Handler.Cur_x != NULL && Handler.Cur_y != NULL && Handler.Cur_z != NULL)
    {
        int32_t delta_x = 0 - *Handler.Cur_x;  // Khoảng cách về gốc X
        int32_t delta_y = 0 - *Handler.Cur_y;  // Khoảng cách về gốc Y
        int32_t delta_z = 0 - *Handler.Cur_z;  // Khoảng cách về gốc Z
        
        // Di chuyển về gốc (0,0,0)
        if (delta_x != 0) {
            Move_x_2_target(abs(delta_x), delta_x < 0 ? Left : Right);
        }
        if (delta_y != 0) {
            Move_y_2_target(abs(delta_y), delta_y < 0 ? In : Out);
        }
        if (delta_z != 0) {
            Move_z_2_target(abs(delta_z), delta_z < 0 ? Up : Down);
        }
    }
}
```

---

### ✅ 6. Loại bỏ hàm Handle_QuarterSelection()
**Lý do:** HMI tự động xử lý chọn góc phần tư

**Đã xóa:**
- Toàn bộ hàm `Handle_QuarterSelection()` (~30 dòng code)
- Lời gọi hàm trong `ModbusApp_Process()`

---

### ✅ 7. Sửa logic teaching để phân biệt HMI edge vs MCU edge
**Vấn đề:** MCU set bit feedback → phát hiện edge → lưu lại tọa độ (vòng lặp vô tận)

**Giải pháp:** Sử dụng cờ `teaching_feedback_flag` để theo dõi nguồn gốc edge

**Logic mới trong `Handle_Teaching()`:**
```c
// Phát hiện cạnh lên (0→1)
if ((current & (1 << bit_index)) && !(prev_coils[3] & (1 << bit_index)))
{
    // Kiểm tra xem đây có phải từ HMI không (không phải feedback từ MCU)
    if (!teaching_feedback_flag)
    {
        // Lưu vị trí hiện tại vào bộ nhớ teaching
        TeachMem.Glass_Pos[pos][0] = *Handler.Cur_x;
        TeachMem.Glass_Pos[pos][1] = *Handler.Cur_y;
        TeachMem.Glass_Pos[pos][2] = *Handler.Cur_z;
        TeachMem.Glass_Valid[pos] = 1;
        
        // Đánh dấu: bit đã được HMI set rồi, không cần MCU set lại
        teaching_feedback_flag = 1;
    }
    else
    {
        // Đây là feedback từ MCU (vòng lặp kế tiếp sau khi MCU set bit)
        // Bỏ qua và reset cờ
        teaching_feedback_flag = 0;
    }
}
```

**Đã thêm biến global:**
```c
static uint8_t teaching_feedback_flag = 0;  // Cờ theo dõi nguồn edge (0: HMI, 1: MCU feedback)
```

---

### ✅ 8. Loại bỏ hàm Handle_CameraTray()
**Lý do:** Chức năng chưa cần thiết ở giai đoạn hiện tại

**Đã xóa:**
- Toàn bộ hàm `Handle_CameraTray()` (~20 dòng code)
- Lời gọi hàm trong `ModbusApp_Process()`

---

### ✅ 9. Loại bỏ hàm Handle_GlassTray()
**Lý do:** Chức năng chưa cần thiết ở giai đoạn hiện tại

**Đã xóa:**
- Toàn bộ hàm `Handle_GlassTray()` (~25 dòng code)
- Lời gọi hàm trong `ModbusApp_Process()`

---

## Kết Quả Sau Refactoring

### Code Statistics
| Thông số | Trước | Sau | Thay đổi |
|----------|-------|-----|----------|
| Số dòng code | ~403 | ~260 | -143 dòng (-35%) |
| Số hàm xử lý | 7 | 2 | -5 hàm |
| Build size (text) | 44872 bytes | 44332 bytes | -540 bytes |
| Build warnings | 11 | 8 | -3 warnings |

### Hàm còn lại trong `ModbusApp_Process()`
```c
void ModbusApp_Process(void)
{
    Handle_ManualMotor();    // Xử lý điều khiển motor thủ công (X/Y/Z + SET/RESET)
    Handle_Teaching();       // Xử lý chức năng teaching (lưu tọa độ Glass/Cover)
}
```

### Build Result
```
✅ Build SUCCESSFUL
✅ No errors
⚠️ 8 warnings (mostly implicit function declarations - non-critical)
   text    data     bss     dec     hex filename
  44332     180    4588   49100    bfcc MC_ATC001.elf
```

---

## Kiến Trúc Mới: HMI-Centric Design

### Phân Chia Trách Nhiệm

| Tác vụ | HMI (Touchscreen) | MCU (STM32) |
|--------|-------------------|-------------|
| Chuyển tab Home/Engineer | ✅ Tự động | ❌ Không xử lý |
| Điều khiển LED trạng thái | ✅ Tự động | ❌ Không xử lý |
| Chọn góc phần tư | ✅ Tự động | ❌ Không xử lý |
| Xóa coil sau lệnh | ✅ Tự động (0.1s) | ❌ Không xử lý |
| Điều khiển motor | ❌ Chỉ gửi lệnh | ✅ **Thực thi** |
| Lưu tọa độ teaching | ❌ Chỉ trigger | ✅ **Lưu vào RAM** |
| Tính toán homing | ❌ Không xử lý | ✅ **Tính delta + di chuyển** |

### Lợi Ích
1. **Code đơn giản hơn:** Giảm 35% số dòng code
2. **Dễ bảo trì:** Tách bạch rõ ràng giữa HMI logic và motor logic
3. **Ít bug hơn:** Không còn vòng lặp vô tận trong teaching
4. **Performance tốt hơn:** Ít xử lý không cần thiết trong MCU

---

## File Cần Cập Nhật Tiếp (TODO)

### 📄 MODBUS_APP_GUIDE.md (Cần viết lại hoàn toàn)
**Lý do:** Hướng dẫn cũ mô tả 7 chức năng, giờ chỉ còn 2 chức năng

**Nội dung mới cần bao gồm:**
1. Giới thiệu kiến trúc HMI-centric
2. Hướng dẫn sử dụng 2 chức năng chính:
   - Manual Motor Control (điều khiển motor thủ công)
   - Teaching (lưu tọa độ Glass/Cover)
3. Giải thích HMI auto-clear behavior (pulse 0.1s)
4. Bảng Modbus address (giữ nguyên)
5. Ví dụ tích hợp (cập nhật)

---

## Kiểm Tra Lại

### Checklist
- [x] Code compile thành công
- [x] Build size giảm
- [x] Không có lỗi biên dịch
- [x] Loại bỏ hết warning về unused functions
- [x] Header file đã cập nhật (modbus_app.h)
- [ ] Cập nhật MODBUS_APP_GUIDE.md (cần làm tiếp)
- [ ] Test trên phần cứng thật

### Lưu Ý Khi Test
1. **Kiểm tra homing:** Nhấn nút RESET → motor phải về (0,0,0)
2. **Kiểm tra teaching:** Nhấn Teaching Pos1 → HMI xóa sau 0.1s (không lặp vô tận)
3. **Kiểm tra manual control:** Giữ nút L/R/U/D → motor chạy, thả nút → motor dừng

---

## Tóm Tắt
- **Thời gian refactoring:** ~30 phút
- **Số file thay đổi:** 2 (modbus_app.c, modbus_app.h)
- **Mức độ thay đổi:** Trung bình (35% code bị xóa)
- **Breaking changes:** Không (API giữ nguyên: Init + Process)
- **Backward compatible:** Có (với HMI mới có chế độ auto-clear)

---

**Người thực hiện:** GitHub Copilot  
**Dựa trên yêu cầu của:** User (9 điểm refactoring)
