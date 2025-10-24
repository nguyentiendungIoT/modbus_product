"""Base client class for Modbus communication."""

from abc import ABC, abstractmethod
from typing import List
import logging

from modbus_product.constants import DEFAULT_UNIT_ID, DEFAULT_TIMEOUT
from modbus_product.exceptions import (
    ModbusConnectionError,
    ModbusReadError,
    ModbusWriteError,
)

logger = logging.getLogger(__name__)


class ModbusClient(ABC):
    """Abstract base class for Modbus clients."""

    def __init__(self, timeout: float = DEFAULT_TIMEOUT):
        """
        Initialize Modbus client.

        Args:
            timeout: Timeout for operations in seconds
        """
        self.timeout = timeout
        self._connected = False
        self._client = None

    @abstractmethod
    def connect(self) -> bool:
        """
        Connect to the Modbus device.

        Returns:
            True if connection successful, False otherwise

        Raises:
            ModbusConnectionError: If connection fails
        """
        pass

    @abstractmethod
    def close(self) -> None:
        """Close the connection to the Modbus device."""
        pass

    def is_connected(self) -> bool:
        """
        Check if client is connected.

        Returns:
            True if connected, False otherwise
        """
        return self._connected

    def read_holding_registers(
        self, address: int, count: int = 1, unit: int = DEFAULT_UNIT_ID
    ) -> List[int]:
        """
        Read holding registers from the device.

        Args:
            address: Starting address to read from
            count: Number of registers to read
            unit: Unit ID of the device

        Returns:
            List of register values

        Raises:
            ModbusReadError: If read operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.read_holding_registers(
                address=address, count=count, slave=unit
            )
            if result.isError():
                raise ModbusReadError(f"Failed to read holding registers: {result}")
            return result.registers
        except Exception as e:
            logger.error(f"Error reading holding registers: {e}")
            raise ModbusReadError(f"Failed to read holding registers: {e}")

    def read_input_registers(
        self, address: int, count: int = 1, unit: int = DEFAULT_UNIT_ID
    ) -> List[int]:
        """
        Read input registers from the device.

        Args:
            address: Starting address to read from
            count: Number of registers to read
            unit: Unit ID of the device

        Returns:
            List of register values

        Raises:
            ModbusReadError: If read operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.read_input_registers(
                address=address, count=count, slave=unit
            )
            if result.isError():
                raise ModbusReadError(f"Failed to read input registers: {result}")
            return result.registers
        except Exception as e:
            logger.error(f"Error reading input registers: {e}")
            raise ModbusReadError(f"Failed to read input registers: {e}")

    def read_coils(
        self, address: int, count: int = 1, unit: int = DEFAULT_UNIT_ID
    ) -> List[bool]:
        """
        Read coils from the device.

        Args:
            address: Starting address to read from
            count: Number of coils to read
            unit: Unit ID of the device

        Returns:
            List of coil values (True/False)

        Raises:
            ModbusReadError: If read operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.read_coils(address=address, count=count, slave=unit)
            if result.isError():
                raise ModbusReadError(f"Failed to read coils: {result}")
            return result.bits[:count]
        except Exception as e:
            logger.error(f"Error reading coils: {e}")
            raise ModbusReadError(f"Failed to read coils: {e}")

    def read_discrete_inputs(
        self, address: int, count: int = 1, unit: int = DEFAULT_UNIT_ID
    ) -> List[bool]:
        """
        Read discrete inputs from the device.

        Args:
            address: Starting address to read from
            count: Number of inputs to read
            unit: Unit ID of the device

        Returns:
            List of input values (True/False)

        Raises:
            ModbusReadError: If read operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.read_discrete_inputs(
                address=address, count=count, slave=unit
            )
            if result.isError():
                raise ModbusReadError(f"Failed to read discrete inputs: {result}")
            return result.bits[:count]
        except Exception as e:
            logger.error(f"Error reading discrete inputs: {e}")
            raise ModbusReadError(f"Failed to read discrete inputs: {e}")

    def write_register(
        self, address: int, value: int, unit: int = DEFAULT_UNIT_ID
    ) -> bool:
        """
        Write a single register.

        Args:
            address: Address to write to
            value: Value to write
            unit: Unit ID of the device

        Returns:
            True if write successful

        Raises:
            ModbusWriteError: If write operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.write_register(
                address=address, value=value, slave=unit
            )
            if result.isError():
                raise ModbusWriteError(f"Failed to write register: {result}")
            return True
        except Exception as e:
            logger.error(f"Error writing register: {e}")
            raise ModbusWriteError(f"Failed to write register: {e}")

    def write_registers(
        self, address: int, values: List[int], unit: int = DEFAULT_UNIT_ID
    ) -> bool:
        """
        Write multiple registers.

        Args:
            address: Starting address to write to
            values: List of values to write
            unit: Unit ID of the device

        Returns:
            True if write successful

        Raises:
            ModbusWriteError: If write operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.write_registers(
                address=address, values=values, slave=unit
            )
            if result.isError():
                raise ModbusWriteError(f"Failed to write registers: {result}")
            return True
        except Exception as e:
            logger.error(f"Error writing registers: {e}")
            raise ModbusWriteError(f"Failed to write registers: {e}")

    def write_coil(
        self, address: int, value: bool, unit: int = DEFAULT_UNIT_ID
    ) -> bool:
        """
        Write a single coil.

        Args:
            address: Address to write to
            value: Value to write (True/False)
            unit: Unit ID of the device

        Returns:
            True if write successful

        Raises:
            ModbusWriteError: If write operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.write_coil(address=address, value=value, slave=unit)
            if result.isError():
                raise ModbusWriteError(f"Failed to write coil: {result}")
            return True
        except Exception as e:
            logger.error(f"Error writing coil: {e}")
            raise ModbusWriteError(f"Failed to write coil: {e}")

    def write_coils(
        self, address: int, values: List[bool], unit: int = DEFAULT_UNIT_ID
    ) -> bool:
        """
        Write multiple coils.

        Args:
            address: Starting address to write to
            values: List of values to write (True/False)
            unit: Unit ID of the device

        Returns:
            True if write successful

        Raises:
            ModbusWriteError: If write operation fails
        """
        if not self._connected:
            raise ModbusConnectionError("Client is not connected")

        try:
            result = self._client.write_coils(
                address=address, values=values, slave=unit
            )
            if result.isError():
                raise ModbusWriteError(f"Failed to write coils: {result}")
            return True
        except Exception as e:
            logger.error(f"Error writing coils: {e}")
            raise ModbusWriteError(f"Failed to write coils: {e}")

    def __enter__(self):
        """Context manager entry."""
        self.connect()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Context manager exit."""
        self.close()
        return False
