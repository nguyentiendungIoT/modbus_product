"""Tests for utility functions."""

import pytest
from modbus_product.utils import (
    bytes_to_int,
    int_to_bytes,
    float_to_registers,
    registers_to_float,
    validate_address,
    validate_register_value,
)


class TestBytesToInt:
    """Tests for bytes_to_int function."""

    def test_single_register(self):
        """Test conversion of single register."""
        assert bytes_to_int([100]) == 100

    def test_two_registers(self):
        """Test conversion of two registers."""
        assert bytes_to_int([1, 0]) == 65536

    def test_multiple_registers(self):
        """Test conversion of multiple registers."""
        assert bytes_to_int([0, 1, 0]) == 65536

    def test_signed_positive(self):
        """Test signed positive value."""
        assert bytes_to_int([100], signed=True) == 100

    def test_signed_negative(self):
        """Test signed negative value."""
        assert bytes_to_int([65535], signed=True) == -1


class TestIntToBytes:
    """Tests for int_to_bytes function."""

    def test_single_register(self):
        """Test conversion to single register."""
        assert int_to_bytes(100, 1) == [100]

    def test_two_registers(self):
        """Test conversion to two registers."""
        assert int_to_bytes(65536, 2) == [1, 0]

    def test_multiple_registers(self):
        """Test conversion to multiple registers."""
        assert int_to_bytes(65536, 3) == [0, 1, 0]


class TestFloatConversion:
    """Tests for float conversion functions."""

    def test_float_to_registers(self):
        """Test converting float to registers."""
        registers = float_to_registers(123.456)
        assert len(registers) == 2
        assert all(isinstance(r, int) for r in registers)

    def test_registers_to_float(self):
        """Test converting registers to float."""
        original = 123.456
        registers = float_to_registers(original)
        result = registers_to_float(registers)
        assert abs(result - original) < 0.001

    def test_float_roundtrip(self):
        """Test roundtrip conversion."""
        test_values = [0.0, 1.0, -1.0, 123.456, -789.012, 3.14159]
        for value in test_values:
            registers = float_to_registers(value)
            result = registers_to_float(registers)
            assert abs(result - value) < 0.001

    def test_registers_to_float_invalid_count(self):
        """Test error on invalid register count."""
        with pytest.raises(ValueError):
            registers_to_float([1])


class TestValidation:
    """Tests for validation functions."""

    def test_validate_address_valid(self):
        """Test valid addresses."""
        assert validate_address(0) is True
        assert validate_address(100) is True
        assert validate_address(65535) is True

    def test_validate_address_invalid_type(self):
        """Test invalid address type."""
        with pytest.raises(ValueError):
            validate_address("100")

    def test_validate_address_negative(self):
        """Test negative address."""
        with pytest.raises(ValueError):
            validate_address(-1)

    def test_validate_address_too_large(self):
        """Test address too large."""
        with pytest.raises(ValueError):
            validate_address(65536)

    def test_validate_register_value_valid(self):
        """Test valid register values."""
        assert validate_register_value(0) is True
        assert validate_register_value(100) is True
        assert validate_register_value(65535) is True

    def test_validate_register_value_invalid_type(self):
        """Test invalid value type."""
        with pytest.raises(ValueError):
            validate_register_value("100")

    def test_validate_register_value_negative(self):
        """Test negative value."""
        with pytest.raises(ValueError):
            validate_register_value(-1)

    def test_validate_register_value_too_large(self):
        """Test value too large."""
        with pytest.raises(ValueError):
            validate_register_value(65536)
