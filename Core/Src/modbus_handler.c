/*
 * modbus_handler.c
 *
 * Move Modbus receive handling out of main.c into a dedicated module.
 */
#include "modbus_handler.h"
#include "modbusSlave.h" // existing modbus library (not modified)
#include "motor_control.h"
#include "main.h" // for huart2
#include <string.h>

// Buffers originally in main.c — modbus library expects extern RxData/TxData
uint8_t RxData[256] = {0};
uint8_t TxData[256] = {0};

// ----- helper: process coils -> motor commands -----
static void Modbus_HandleServoCoils(void)
{
    // X axis
    if (GET_COIL(COIL_X_NEG) && !GET_COIL(COIL_X_POS)) {
        Motor_X_Move(0, 0);
    } else if (GET_COIL(COIL_X_POS) && !GET_COIL(COIL_X_NEG)) {
        Motor_X_Move(1, 0);
    } else {
        Motor_X_Stop();
    }

    // Y axis
    if (GET_COIL(COIL_Y_POS) && !GET_COIL(COIL_Y_NEG)) {
        Motor_Y_Move(1, 0);
    } else if (GET_COIL(COIL_Y_NEG) && !GET_COIL(COIL_Y_POS)) {
        Motor_Y_Move(0, 0);
    } else {
        Motor_Y_Stop();
    }

    // Z axis
    if (GET_COIL(COIL_Z_POS) && !GET_COIL(COIL_Z_NEG)) {
        Motor_Z_Move(1, 0);
    } else if (GET_COIL(COIL_Z_NEG) && !GET_COIL(COIL_Z_POS)) {
        Motor_Z_Move(0, 0);
    } else {
        Motor_Z_Stop();
    }
}

void ModbusHandler_Init(void)
{
    // reset coils and buffers
    memset(RxData, 0, sizeof(RxData));
    memset((void*)Coils_Database, 0, sizeof(Coils_Database));
    // start receive to idle on UART2 (Modbus)
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxData, sizeof(RxData));
}

// UART callback: called by HAL when a message is received (ReceiveToIdle)
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2 && RxData[0] == SLAVE_ID)
    {
        switch (RxData[1]){
        case 0x03: readHoldingRegs(); break;      // Read Holding Registers
        case 0x04: readInputRegs(); break;        // Read Input Registers
        case 0x01: readCoils(); break;            // Read Coils
        case 0x02: readInputs(); break;           // Read Discrete Inputs
        case 0x06: writeSingleReg(); break;       // Write Single Register
        case 0x10: writeHoldingRegs(); break;     // Write Multiple Registers
        case 0x05: writeSingleCoil(); break;      // Write Single Coil
        case 0x0F: writeMultiCoils(); break;      // Write Multiple Coils
        default: modbusException(ILLEGAL_FUNCTION); break;
        }

        // Servo control via coils
        Modbus_HandleServoCoils();
    }

    // restart receive for next message
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxData, sizeof(RxData));
}
