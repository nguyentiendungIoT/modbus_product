"""Tests for constants module."""

from modbus_product.constants import (
    READ_COILS,
    READ_DISCRETE_INPUTS,
    READ_HOLDING_REGISTERS,
    READ_INPUT_REGISTERS,
    WRITE_SINGLE_COIL,
    WRITE_SINGLE_REGISTER,
    WRITE_MULTIPLE_COILS,
    WRITE_MULTIPLE_REGISTERS,
    DEFAULT_TCP_PORT,
    DEFAULT_UNIT_ID,
    DEFAULT_TIMEOUT,
    DEFAULT_BAUDRATE,
    MAX_REGISTER_COUNT,
    MAX_COIL_COUNT,
)


class TestConstants:
    """Tests for Modbus constants."""

    def test_function_codes(self):
        """Test function code constants."""
        assert READ_COILS == 0x01
        assert READ_DISCRETE_INPUTS == 0x02
        assert READ_HOLDING_REGISTERS == 0x03
        assert READ_INPUT_REGISTERS == 0x04
        assert WRITE_SINGLE_COIL == 0x05
        assert WRITE_SINGLE_REGISTER == 0x06
        assert WRITE_MULTIPLE_COILS == 0x0F
        assert WRITE_MULTIPLE_REGISTERS == 0x10

    def test_default_values(self):
        """Test default value constants."""
        assert DEFAULT_TCP_PORT == 502
        assert DEFAULT_UNIT_ID == 1
        assert DEFAULT_TIMEOUT == 3.0
        assert DEFAULT_BAUDRATE == 9600

    def test_limits(self):
        """Test limit constants."""
        assert MAX_REGISTER_COUNT == 125
        assert MAX_COIL_COUNT == 2000
