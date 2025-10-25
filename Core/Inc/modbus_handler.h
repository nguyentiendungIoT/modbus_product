/*
 * modbus_handler.h
 *
 * Lightweight wrapper that owns the Modbus Rx/Tx buffers and the UART callback.
 * It calls the existing Modbus library functions (modbusSlave.*) and the motor API.
 */
#ifndef INC_MODBUS_HANDLER_H_
#define INC_MODBUS_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void ModbusHandler_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_MODBUS_HANDLER_H_ */
