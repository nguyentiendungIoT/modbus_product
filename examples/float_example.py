"""
Example: Working with Float Values

This example demonstrates how to read and write float values using
the utility functions.
"""

from modbus_product import ModbusTCPClient
from modbus_product.utils import float_to_registers, registers_to_float


def main():
    # Configure connection parameters
    HOST = "192.168.1.100"
    PORT = 502
    UNIT_ID = 1

    with ModbusTCPClient(host=HOST, port=PORT) as client:
        print("Connected to Modbus device")

        # Write a float value (temperature, for example)
        temperature = 23.456
        print(f"\n--- Writing Float Value: {temperature} ---")

        # Convert float to two registers
        registers = float_to_registers(temperature)
        print(f"Float {temperature} converts to registers: {registers}")

        # Write the registers
        client.write_registers(address=100, values=registers, unit=UNIT_ID)
        print("Float value written successfully")

        # Read back the float value
        print("\n--- Reading Float Value ---")
        read_registers = client.read_holding_registers(
            address=100, count=2, unit=UNIT_ID
        )
        print(f"Read registers: {read_registers}")

        # Convert registers back to float
        read_temperature = registers_to_float(read_registers)
        print(f"Converted to float: {read_temperature}")

        # Verify accuracy
        if abs(read_temperature - temperature) < 0.001:
            print("✓ Float read/write successful!")
        else:
            print("✗ Float values don't match")


if __name__ == "__main__":
    main()
