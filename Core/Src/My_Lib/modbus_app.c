/*
 * modbus_app.c
 *
 *  Created on: Oct 31, 2025
 *      Author: MCNEX
 *  Description: Mô-đun xử lý logic ứng dụng Modbus (Modbus Application Layer)
 *               Chịu trách nhiệm điều khiển motor, teaching, HMI, queue processing
 */

#include "My_Lib/modbus_app.h"
#include "My_Lib/modbusSlave.h"
#include "My_Lib/motor.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>

// ========== MACRO TỐI ƯU (OPTIMIZATION MACROS) ==========
// Macro kiểm tra bit coil theo địa chỉ Modbus (tự động tính byte và bit position)
#define GET_COIL(addr) ((Coils_Database[(addr)/8] >> ((addr)%8)) & 0x01)

// Macro set bit coil theo địa chỉ Modbus (để feedback cho HMI)
#define SET_COIL(addr) (Coils_Database[(addr)/8] |= (1 << ((addr)%8)))

// ========== BIẾN TOÀN CỤC (GLOBAL VARIABLES) ==========
TeachingMemory_t TeachMem = {0};  // Bộ nhớ lưu vị trí teaching (Glass và Cover)

// Shadow registers để phát hiện thay đổi cạnh lên (rising edge) của coils
static uint8_t prev_coils[4] = {0};  // Lưu trạng thái trước của Byte 0-3

// Feedback flags: Tracking teaching success để re-set coil nếu HMI clear
// Bit 0-2: Glass Pos1-3, Bit 3-5: Cover Pos1-3
static uint8_t teaching_feedback_status = 0;  // Flag cho các vị trí đã teaching thành công

// ========== KHAI BÁO HÀM PRIVATE (PRIVATE FUNCTION PROTOTYPES) ==========
static void Handle_ManualMotor(void);        // Xử lý nút điều khiển motor thủ công
static void Handle_Teaching(void);           // Xử lý chức năng teaching (SET/RESET)

// ========== TRIỂN KHAI API CÔNG KHAI (PUBLIC API IMPLEMENTATION) ==========

void ModbusApp_Init(void)
{
    // Khởi tạo bộ nhớ teaching (xóa hết về 0)
    memset(&TeachMem, 0, sizeof(TeachMem));
    
    // Khởi tạo shadow registers (xóa hết về 0)
    memset(prev_coils, 0, sizeof(prev_coils));
    
    // Khởi tạo teaching feedback status (xóa hết về 0)
    teaching_feedback_status = 0;
}

void ModbusApp_Process(void)
{
    // Xử lý coils (các nút nhấn từ HMI)
    Handle_ManualMotor();        // Xử lý điều khiển motor thủ công
    Handle_Teaching();           // Xử lý chức năng teaching (SET/RESET)
}

void ModbusApp_SetCameraColor(uint8_t index, uint16_t color)
{
    if (index < 1 || index > 25) return;  // Kiểm tra index hợp lệ (1-25)
    Holding_Registers_Database[REG_CAMERA_CELL_START + index - 1] = color;  // Ghi màu vào thanh ghi
}

void ModbusApp_SetGlassColor(uint8_t index, uint16_t color)
{
    if (index < 1 || index > 25) return;  // Kiểm tra index hợp lệ (1-25)
    Holding_Registers_Database[REG_GLASS_CELL_START + index - 1] = color;   // Ghi màu vào thanh ghi
}

// ========== TRIỂN KHAI HÀM PRIVATE (PRIVATE FUNCTION IMPLEMENTATIONS) ==========

static void Handle_ManualMotor(void)
{
    uint8_t current = Coils_Database[1];  // Đọc trạng thái coils byte 1
    
    // ===== TRỤC X (Kích hoạt theo mức - level-triggered) =====
    if (GET_COIL(COIL_MOTOR_X_LEFT))  // Nếu nút X_LEFT đang được giữ
    {
        if (Motor_x.state == Stop) {  // Chỉ chạy khi motor đang dừng
            Move_x(Left, 100);  // Di chuyển trái với tần số 100Hz
        }
    }
    else if (GET_COIL(COIL_MOTOR_X_RIGHT))  // Nếu nút X_RIGHT đang được giữ
    {
        if (Motor_x.state == Stop) {
            Move_x(Right, 100);  // Di chuyển phải với tần số 100Hz
        }
    }
    else  // Nếu không có nút nào được giữ
    {
        if (Motor_x.state == Run_by_Hand) {  // Nếu đang ở chế độ thủ công
            Stop_x();  // Dừng motor X
        }
    }
    
    // ===== TRỤC Y (Kích hoạt theo mức - level-triggered) =====
    if (GET_COIL(COIL_MOTOR_Y_IN))  // Nếu nút Y_IN đang được giữ
    {
        if (Motor_y.state == Stop) {
            Move_y(In, 100);  // Di chuyển vào trong với tần số 100Hz
        }
    }
    else if (GET_COIL(COIL_MOTOR_Y_OUT))  // Nếu nút Y_OUT đang được giữ
    {
        if (Motor_y.state == Stop) {
            Move_y(Out, 100);  // Di chuyển ra ngoài với tần số 100Hz
        }
    }
    else  // Nếu không có nút nào được giữ
    {
        if (Motor_y.state == Run_by_Hand) {
            Stop_y();  // Dừng motor Y
        }
    }
    
    // ===== TRỤC Z (Kích hoạt theo mức - level-triggered) =====
    if (GET_COIL(COIL_MOTOR_Z_UP))  // Nếu nút Z_UP đang được giữ
    {
        if (Motor_z.state == Stop) {
            Move_z(Up, 100);  // Di chuyển lên trên với tần số 100Hz
        }
    }
    else if (GET_COIL(COIL_MOTOR_Z_DOWN))  // Nếu nút Z_DOWN đang được giữ
    {
        if (Motor_z.state == Stop) {
            Move_z(Down, 100);  // Di chuyển xuống dưới với tần số 100Hz
        }
    }
    else  // Nếu không có nút nào được giữ
    {
        if (Motor_z.state == Run_by_Hand) {
            Stop_z();  // Dừng motor Z
        }
    }
    
    // ===== NÚT SET (Kích hoạt theo cạnh lên - edge-triggered) =====
    if (GET_COIL(COIL_MOTOR_SET) && !(prev_coils[1] & (1 << (COIL_MOTOR_SET - 8))))
    {
        if (Motor_x.state == Stop && Motor_y.state == Stop && Motor_z.state == Stop)  // Chỉ thực hiện khi tất cả motor dừng
        {
            // Đọc tọa độ đích từ thanh ghi Modbus
            uint16_t target_x = Holding_Registers_Database[REG_COORD_X];
            uint16_t target_y = Holding_Registers_Database[REG_COORD_Y];
            uint16_t target_z = Holding_Registers_Database[REG_COORD_Z];
            
            // Tính khoảng cách cần di chuyển (kiểm tra con trỏ NULL trước)
            if (Handler.Cur_x != NULL && Handler.Cur_y != NULL && Handler.Cur_z != NULL)
            {
                int32_t delta_x = target_x - *Handler.Cur_x;  // Khoảng cách X
                int32_t delta_y = target_y - *Handler.Cur_y;  // Khoảng cách Y
                int32_t delta_z = target_z - *Handler.Cur_z;  // Khoảng cách Z
                
                // Di chuyển đến tọa độ đích (chỉ di chuyển các trục có delta khác 0)
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
    }
    
    // ===== NÚT RESET (Kích hoạt theo cạnh lên - edge-triggered) =====
    if (GET_COIL(COIL_MOTOR_RESET) && !(prev_coils[1] & (1 << (COIL_MOTOR_RESET - 8))))
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
    
    prev_coils[1] = current;  // Lưu trạng thái hiện tại
}

static void Handle_Teaching(void)
{
    uint8_t current = Coils_Database[3];  // Đọc trạng thái coils byte 3
    
    // ===== TEACHING VỊ TRÍ GLASS (00025-00027: Pos1, Pos2, Pos3) =====
    for (int pos = 0; pos < 3; pos++)
    {
        uint16_t coil_addr = COIL_TEACH_GLASS_POS1 + pos;  // Địa chỉ coil (25, 26, 27)
        uint8_t bit_index = coil_addr - 24;  // Bit index trong byte 3
        
        // Phát hiện cạnh lên (0→1)
        if ((current & (1 << bit_index)) && !(prev_coils[3] & (1 << bit_index)))
        {
            // Lưu vị trí hiện tại vào bộ nhớ teaching
            if (Handler.Cur_x != NULL && Handler.Cur_y != NULL && Handler.Cur_z != NULL)
            {
                TeachMem.Glass_Pos[pos][0] = *Handler.Cur_x;  // Lưu tọa độ X
                TeachMem.Glass_Pos[pos][1] = *Handler.Cur_y;  // Lưu tọa độ Y
                TeachMem.Glass_Pos[pos][2] = *Handler.Cur_z;  // Lưu tọa độ Z
                TeachMem.Glass_Valid[pos] = 1;  // Đánh dấu vị trí này đã lưu
                
                // ===== Set flag feedback để MCU giữ coil=1 =====
                teaching_feedback_status |= (1 << pos);  // Set bit 0-2 cho Glass Pos1-3
                
                // ===== Set coil để HMI hiển thị màu xanh (feedback) =====
                SET_COIL(coil_addr);  // Ghi 1 vào coil 00025/00026/00027
            }
        }
        
        // ===== RE-SET COIL NẾU HMI VÔ TÌNH CLEAR (Race condition protection) =====
        // Nếu đã teaching thành công (flag=1) nhưng coil bị clear về 0 → Re-set lại
        if ((teaching_feedback_status & (1 << pos)) && !GET_COIL(coil_addr))
        {
            SET_COIL(coil_addr);  // Re-set coil về 1 để giữ màu xanh
        }
    }
    
    // ===== RESET VỊ TRÍ GLASS (00028) =====
    if (GET_COIL(COIL_TEACH_GLASS_RESET) && !(prev_coils[3] & (1 << (COIL_TEACH_GLASS_RESET - 24))))
    {
        // Xóa tất cả vị trí Glass đã lưu
        memset(TeachMem.Glass_Pos, 0, sizeof(TeachMem.Glass_Pos));
        memset(TeachMem.Glass_Valid, 0, sizeof(TeachMem.Glass_Valid));
        
        // Xóa feedback flags cho Glass (bit 0-2)
        teaching_feedback_status &= ~0x07;  // Clear bit 0-2
        
        // Xóa tất cả coils 00025-00027 (chuyển nút về màu đen)
        for (int i = 0; i < 3; i++) {
            Coils_Database[3] &= ~(1 << (COIL_TEACH_GLASS_POS1 - 24 + i));
        }
    }
    
    // ===== TEACHING VỊ TRÍ COVER (00029-00031: Pos1, Pos2, Pos3) =====
    for (int pos = 0; pos < 3; pos++)
    {
        uint16_t coil_addr = COIL_TEACH_COVER_POS1 + pos;  // Địa chỉ coil (29, 30, 31)
        uint8_t bit_index = coil_addr - 24;  // Bit index trong byte 3
        
        // Phát hiện cạnh lên (0→1)
        if ((current & (1 << bit_index)) && !(prev_coils[3] & (1 << bit_index)))
        {
            // Lưu vị trí hiện tại vào bộ nhớ teaching
            if (Handler.Cur_x != NULL && Handler.Cur_y != NULL && Handler.Cur_z != NULL)
            {
                TeachMem.Cover_Pos[pos][0] = *Handler.Cur_x;
                TeachMem.Cover_Pos[pos][1] = *Handler.Cur_y;
                TeachMem.Cover_Pos[pos][2] = *Handler.Cur_z;
                TeachMem.Cover_Valid[pos] = 1;
                
                // ===== Set flag feedback để MCU giữ coil=1 =====
                teaching_feedback_status |= (1 << (pos + 3));  // Set bit 3-5 cho Cover Pos1-3
                
                // ===== Set coil để HMI hiển thị màu xanh (feedback) =====
                SET_COIL(coil_addr);  // Ghi 1 vào coil 00029/00030/00031
            }
        }
        
        // ===== RE-SET COIL NẾU HMI VÔ TÌNH CLEAR (Race condition protection) =====
        // Nếu đã teaching thành công (flag=1) nhưng coil bị clear về 0 → Re-set lại
        if ((teaching_feedback_status & (1 << (pos + 3))) && !GET_COIL(coil_addr))
        {
            SET_COIL(coil_addr);  // Re-set coil về 1 để giữ màu xanh
        }
    }
    
    // ===== RESET VỊ TRÍ COVER (00032) =====
    if (GET_COIL(COIL_TEACH_COVER_RESET) && !(prev_coils[3] & (1 << (COIL_TEACH_COVER_RESET - 24))))
    {
        // Xóa tất cả vị trí Cover đã lưu
        memset(TeachMem.Cover_Pos, 0, sizeof(TeachMem.Cover_Pos));
        memset(TeachMem.Cover_Valid, 0, sizeof(TeachMem.Cover_Valid));
        
        // Xóa feedback flags cho Cover (bit 3-5)
        teaching_feedback_status &= ~0x38;  // Clear bit 3-5
        
        // Xóa tất cả coils 00029-00031 (chuyển nút về màu đen)
        for (int i = 0; i < 3; i++) {
            Coils_Database[3] &= ~(1 << (COIL_TEACH_COVER_POS1 - 24 + i));
        }
    }
    
    prev_coils[3] = current;  // Lưu trạng thái hiện tại
}
