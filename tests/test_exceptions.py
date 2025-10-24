"""Tests for custom exceptions."""

from modbus_product.exceptions import (
    ModbusException,
    ModbusConnectionError,
    ModbusReadError,
    ModbusWriteError,
    ModbusTimeoutError,
    ModbusInvalidAddressError,
    ModbusInvalidValueError,
)


class TestExceptions:
    """Tests for Modbus exceptions."""

    def test_base_exception(self):
        """Test base ModbusException."""
        exc = ModbusException("Test error")
        assert str(exc) == "Test error"
        assert isinstance(exc, Exception)

    def test_connection_error(self):
        """Test ModbusConnectionError."""
        exc = ModbusConnectionError("Connection failed")
        assert str(exc) == "Connection failed"
        assert isinstance(exc, ModbusException)

    def test_read_error(self):
        """Test ModbusReadError."""
        exc = ModbusReadError("Read failed")
        assert str(exc) == "Read failed"
        assert isinstance(exc, ModbusException)

    def test_write_error(self):
        """Test ModbusWriteError."""
        exc = ModbusWriteError("Write failed")
        assert str(exc) == "Write failed"
        assert isinstance(exc, ModbusException)

    def test_timeout_error(self):
        """Test ModbusTimeoutError."""
        exc = ModbusTimeoutError("Operation timed out")
        assert str(exc) == "Operation timed out"
        assert isinstance(exc, ModbusException)

    def test_invalid_address_error(self):
        """Test ModbusInvalidAddressError."""
        exc = ModbusInvalidAddressError("Invalid address")
        assert str(exc) == "Invalid address"
        assert isinstance(exc, ModbusException)

    def test_invalid_value_error(self):
        """Test ModbusInvalidValueError."""
        exc = ModbusInvalidValueError("Invalid value")
        assert str(exc) == "Invalid value"
        assert isinstance(exc, ModbusException)

    def test_exception_hierarchy(self):
        """Test exception hierarchy."""
        # All custom exceptions should inherit from ModbusException
        exceptions = [
            ModbusConnectionError,
            ModbusReadError,
            ModbusWriteError,
            ModbusTimeoutError,
            ModbusInvalidAddressError,
            ModbusInvalidValueError,
        ]
        for exc_class in exceptions:
            exc = exc_class("Test")
            assert isinstance(exc, ModbusException)
            assert isinstance(exc, Exception)
