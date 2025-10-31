/*
 * modbus_app.h
 *
 *  Created on: Oct 31, 2025
 *      Author: MCNEX
 *  Description: Modbus Application Layer - HMI to Motor Control Interface
 */

#ifndef INC_MY_LIB_MODBUS_APP_H_
#define INC_MY_LIB_MODBUS_APP_H_

#include "stdint.h"
#include "stdbool.h"

// ========== MODBUS ADDRESS MAPPING ==========

// --- COILS (0x) - Bit addresses ---
// Byte 0: Tab Navigation (00001-00008)
#define COIL_TAB_HOME           0   // 00001
#define COIL_TAB_ENGINE         1   // 00002

// Byte 1: Manual Motor Control (00009-00016)
#define COIL_MOTOR_X_LEFT       8   // 00009
#define COIL_MOTOR_X_RIGHT      9   // 00010
#define COIL_MOTOR_Y_IN         10  // 00011
#define COIL_MOTOR_Y_OUT        11  // 00012
#define COIL_MOTOR_Z_UP         12  // 00013
#define COIL_MOTOR_Z_DOWN       13  // 00014
#define COIL_MOTOR_SET          14  // 00015
#define COIL_MOTOR_RESET        15  // 00016

// Byte 2: Quarter Selection & Status (00017-00024)
#define COIL_QUARTER_1          16  // 00017
#define COIL_QUARTER_2          17  // 00018
#define COIL_QUARTER_3          18  // 00019
#define COIL_QUARTER_4          19  // 00020
#define COIL_LED_GREEN          20  // 00021 (VXL → HMI)
#define COIL_LED_RED            21  // 00022 (VXL → HMI)
#define COIL_RESET_HOME         22  // 00023

// Byte 3: Teaching Mode (00025-00032)
#define COIL_TEACH_GLASS_POS1   24  // 00025
#define COIL_TEACH_GLASS_POS2   25  // 00026
#define COIL_TEACH_GLASS_POS3   26  // 00027
#define COIL_TEACH_GLASS_RESET  27  // 00028
#define COIL_TEACH_COVER_POS1   28  // 00029
#define COIL_TEACH_COVER_POS2   29  // 00030
#define COIL_TEACH_COVER_POS3   30  // 00031
#define COIL_TEACH_COVER_RESET  31  // 00032

// --- HOLDING REGISTERS (4x) - 16-bit addresses ---
// Coordinate Input (400001-400003)
#define REG_COORD_X             0   // 400001
#define REG_COORD_Y             1   // 400002
#define REG_COORD_Z             2   // 400003

// Camera Tray (400010-400035)
#define REG_CAMERA_ACTIVE       9   // 400010
#define REG_CAMERA_CELL_START   10  // 400011-400035 (25 cells)

// Glass Tray (400100-400296)
#define REG_GLASS_FLAG          99  // 400100
#define REG_GLASS_CELL_START    100 // 400101-400296 (196 cells)

// ========== COLOR CODES ==========
#define COLOR_CAMERA_WHITE      0   // Default
#define COLOR_CAMERA_GREEN      1   // User selected
#define COLOR_CAMERA_BLACK      2   // Already picked

#define COLOR_GLASS_WHITE       0   // Picked
#define COLOR_GLASS_RED         1   // NG (defect)
#define COLOR_GLASS_GRAY        2   // Not inspected
#define COLOR_GLASS_YELLOW      3   // User selected start point

// ========== DATA STRUCTURES ==========

// Teaching memory structure
typedef struct {
    uint16_t Glass_Pos[3][3];  // [pos_id][x,y,z]
    uint16_t Cover_Pos[3][3];
    uint8_t Glass_Valid[3];    // bit flags: 0=not set, 1=set
    uint8_t Cover_Valid[3];
} TeachingMemory_t;

extern TeachingMemory_t TeachMem;

// ========== MAIN API FUNCTIONS ==========

/**
 * @brief Initialize Modbus Application Layer
 */
void ModbusApp_Init(void);

/**
 * @brief Main processing function - Call in UART callback after Modbus protocol handling
 */
void ModbusApp_Process(void);

#endif /* INC_MY_LIB_MODBUS_APP_H_ */
