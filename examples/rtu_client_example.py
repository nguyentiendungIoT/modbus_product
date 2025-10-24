"""
Example: Basic Modbus RTU Client Usage

This example demonstrates how to use the ModbusRTUClient to communicate
with a Modbus RTU device over serial connection.
"""

from modbus_product import ModbusRTUClient


def main():
    # Configure serial connection parameters
    PORT = "/dev/ttyUSB0"  # Change to your serial port (COM1, COM2, etc. on Windows)
    BAUDRATE = 9600
    UNIT_ID = 1

    # Create and connect client
    client = ModbusRTUClient(
        port=PORT, baudrate=BAUDRATE, bytesize=8, parity="N", stopbits=1, timeout=3.0
    )

    try:
        # Connect to the device
        print(f"Connecting to {PORT} at {BAUDRATE} baud...")
        client.connect()
        print("Connected successfully!")

        # Read holding registers
        print("\n--- Reading Holding Registers ---")
        registers = client.read_holding_registers(address=0, count=10, unit=UNIT_ID)
        print(f"Registers 0-9: {registers}")

        # Write a single register
        print("\n--- Writing Single Register ---")
        client.write_register(address=0, value=456, unit=UNIT_ID)
        print("Wrote value 456 to register 0")

        # Verify the write
        result = client.read_holding_registers(address=0, count=1, unit=UNIT_ID)
        print(f"Verified value: {result[0]}")

        # Read input registers
        print("\n--- Reading Input Registers ---")
        input_regs = client.read_input_registers(address=0, count=5, unit=UNIT_ID)
        print(f"Input registers 0-4: {input_regs}")

        # Read coils
        print("\n--- Reading Coils ---")
        coils = client.read_coils(address=0, count=8, unit=UNIT_ID)
        print(f"Coils 0-7: {coils}")

        # Write a coil
        print("\n--- Writing Coil ---")
        client.write_coil(address=0, value=True, unit=UNIT_ID)
        print("Set coil 0 to True")

    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Always close the connection
        client.close()
        print("\nConnection closed")


if __name__ == "__main__":
    main()
