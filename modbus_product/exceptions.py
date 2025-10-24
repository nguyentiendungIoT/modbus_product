"""Custom exceptions for Modbus operations."""


class ModbusException(Exception):
    """Base exception for all Modbus-related errors."""

    pass


class ModbusConnectionError(ModbusException):
    """Exception raised when connection to Modbus device fails."""

    pass


class ModbusReadError(ModbusException):
    """Exception raised when reading from Modbus device fails."""

    pass


class ModbusWriteError(ModbusException):
    """Exception raised when writing to Modbus device fails."""

    pass


class ModbusTimeoutError(ModbusException):
    """Exception raised when Modbus operation times out."""

    pass


class ModbusInvalidAddressError(ModbusException):
    """Exception raised when an invalid address is provided."""

    pass


class ModbusInvalidValueError(ModbusException):
    """Exception raised when an invalid value is provided."""

    pass
