"""Modbus RTU client implementation."""

import logging

from pymodbus.client import ModbusSerialClient as PyModbusSerialClient

from modbus_product.client import ModbusClient
from modbus_product.constants import (
    DEFAULT_BAUDRATE,
    DEFAULT_BYTESIZE,
    DEFAULT_PARITY,
    DEFAULT_STOPBITS,
    DEFAULT_TIMEOUT,
)
from modbus_product.exceptions import ModbusConnectionError

logger = logging.getLogger(__name__)


class ModbusRTUClient(ModbusClient):
    """Modbus RTU client for serial communication."""

    def __init__(
        self,
        port: str,
        baudrate: int = DEFAULT_BAUDRATE,
        bytesize: int = DEFAULT_BYTESIZE,
        parity: str = DEFAULT_PARITY,
        stopbits: int = DEFAULT_STOPBITS,
        timeout: float = DEFAULT_TIMEOUT,
    ):
        """
        Initialize Modbus RTU client.

        Args:
            port: Serial port name (e.g., '/dev/ttyUSB0' on Linux, 'COM1' on Windows)
            baudrate: Baud rate (default: 9600)
            bytesize: Number of data bits (default: 8)
            parity: Parity setting - 'N' (None), 'E' (Even), 'O' (Odd) (default: 'N')
            stopbits: Number of stop bits (default: 1)
            timeout: Timeout for operations in seconds
        """
        super().__init__(timeout=timeout)
        self.port = port
        self.baudrate = baudrate
        self.bytesize = bytesize
        self.parity = parity
        self.stopbits = stopbits
        logger.info(f"Initialized ModbusRTUClient for {port} at {baudrate} baud")

    def connect(self) -> bool:
        """
        Connect to the Modbus RTU device.

        Returns:
            True if connection successful

        Raises:
            ModbusConnectionError: If connection fails
        """
        try:
            self._client = PyModbusSerialClient(
                port=self.port,
                baudrate=self.baudrate,
                bytesize=self.bytesize,
                parity=self.parity,
                stopbits=self.stopbits,
                timeout=self.timeout,
            )
            if self._client.connect():
                self._connected = True
                logger.info(f"Connected to Modbus RTU device at {self.port}")
                return True
            else:
                raise ModbusConnectionError(f"Failed to connect to {self.port}")
        except Exception as e:
            logger.error(f"Connection error: {e}")
            raise ModbusConnectionError(f"Failed to connect: {e}")

    def close(self) -> None:
        """Close the connection to the Modbus RTU device."""
        if self._client:
            self._client.close()
            self._connected = False
            logger.info(f"Closed connection to {self.port}")

    def __repr__(self) -> str:
        """Return string representation of the client."""
        status = "connected" if self._connected else "disconnected"
        return (
            f"ModbusRTUClient(port='{self.port}', "
            f"baudrate={self.baudrate}, status='{status}')"
        )
