"""Modbus TCP client implementation."""

import logging

from pymodbus.client import ModbusTcpClient as PyModbusTcpClient

from modbus_product.client import ModbusClient
from modbus_product.constants import DEFAULT_TCP_PORT, DEFAULT_TIMEOUT
from modbus_product.exceptions import ModbusConnectionError

logger = logging.getLogger(__name__)


class ModbusTCPClient(ModbusClient):
    """Modbus TCP client for communication over TCP/IP network."""

    def __init__(
        self,
        host: str,
        port: int = DEFAULT_TCP_PORT,
        timeout: float = DEFAULT_TIMEOUT,
    ):
        """
        Initialize Modbus TCP client.

        Args:
            host: IP address or hostname of the Modbus device
            port: TCP port number (default: 502)
            timeout: Timeout for operations in seconds
        """
        super().__init__(timeout=timeout)
        self.host = host
        self.port = port
        logger.info(f"Initialized ModbusTCPClient for {host}:{port}")

    def connect(self) -> bool:
        """
        Connect to the Modbus TCP device.

        Returns:
            True if connection successful

        Raises:
            ModbusConnectionError: If connection fails
        """
        try:
            self._client = PyModbusTcpClient(
                host=self.host, port=self.port, timeout=self.timeout
            )
            if self._client.connect():
                self._connected = True
                logger.info(
                    f"Connected to Modbus TCP device at {self.host}:{self.port}"
                )
                return True
            else:
                raise ModbusConnectionError(
                    f"Failed to connect to {self.host}:{self.port}"
                )
        except Exception as e:
            logger.error(f"Connection error: {e}")
            raise ModbusConnectionError(f"Failed to connect: {e}")

    def close(self) -> None:
        """Close the connection to the Modbus TCP device."""
        if self._client:
            self._client.close()
            self._connected = False
            logger.info(f"Closed connection to {self.host}:{self.port}")

    def __repr__(self) -> str:
        """Return string representation of the client."""
        status = "connected" if self._connected else "disconnected"
        return (
            f"ModbusTCPClient(host='{self.host}', port={self.port}, status='{status}')"
        )
