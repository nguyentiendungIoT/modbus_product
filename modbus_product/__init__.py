"""
Modbus Product - A professional Python library for Modbus communication.

This package provides easy-to-use clients for Modbus TCP and RTU protocols,
designed for industrial IoT applications.
"""

__version__ = "0.1.0"
__author__ = "Nguyen Tien Dung"
__email__ = "support@modbusproduct.com"

from modbus_product.tcp_client import ModbusTCPClient
from modbus_product.rtu_client import ModbusRTUClient
from modbus_product.exceptions import (
    ModbusException,
    ModbusConnectionError,
    ModbusReadError,
    ModbusWriteError,
    ModbusTimeoutError,
)

__all__ = [
    "ModbusTCPClient",
    "ModbusRTUClient",
    "ModbusException",
    "ModbusConnectionError",
    "ModbusReadError",
    "ModbusWriteError",
    "ModbusTimeoutError",
]
