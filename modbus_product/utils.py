"""Utility functions for Modbus operations."""

from typing import List


def bytes_to_int(data: List[int], signed: bool = False) -> int:
    """
    Convert a list of register values to an integer.

    Args:
        data: List of register values (16-bit each)
        signed: Whether to interpret as signed integer

    Returns:
        Integer value
    """
    value = 0
    for i, reg in enumerate(data):
        value |= reg << (16 * (len(data) - 1 - i))

    if signed and len(data) > 0:
        bit_length = len(data) * 16
        if value >= (1 << (bit_length - 1)):
            value -= 1 << bit_length

    return value


def int_to_bytes(value: int, register_count: int = 2) -> List[int]:
    """
    Convert an integer to a list of register values.

    Args:
        value: Integer value to convert
        register_count: Number of registers to use

    Returns:
        List of register values (16-bit each)
    """
    registers = []
    for i in range(register_count):
        shift = 16 * (register_count - 1 - i)
        registers.append((value >> shift) & 0xFFFF)
    return registers


def float_to_registers(value: float) -> List[int]:
    """
    Convert a float to two register values (32-bit IEEE 754).

    Args:
        value: Float value to convert

    Returns:
        List of two register values
    """
    import struct

    byte_data = struct.pack(">f", value)
    return [
        int.from_bytes(byte_data[0:2], byteorder="big"),
        int.from_bytes(byte_data[2:4], byteorder="big"),
    ]


def registers_to_float(registers: List[int]) -> float:
    """
    Convert two register values to a float (32-bit IEEE 754).

    Args:
        registers: List of two register values

    Returns:
        Float value
    """
    import struct

    if len(registers) != 2:
        raise ValueError("Expected 2 registers for float conversion")

    byte_data = registers[0].to_bytes(2, byteorder="big") + registers[1].to_bytes(
        2, byteorder="big"
    )
    return struct.unpack(">f", byte_data)[0]


def validate_address(address: int) -> bool:
    """
    Validate a Modbus address.

    Args:
        address: Address to validate

    Returns:
        True if valid

    Raises:
        ValueError: If address is invalid
    """
    if not isinstance(address, int):
        raise ValueError(f"Address must be an integer, got {type(address)}")
    if address < 0 or address > 65535:
        raise ValueError(f"Address must be between 0 and 65535, got {address}")
    return True


def validate_register_value(value: int) -> bool:
    """
    Validate a register value.

    Args:
        value: Value to validate

    Returns:
        True if valid

    Raises:
        ValueError: If value is invalid
    """
    if not isinstance(value, int):
        raise ValueError(f"Value must be an integer, got {type(value)}")
    if value < 0 or value > 65535:
        raise ValueError(f"Value must be between 0 and 65535, got {value}")
    return True
