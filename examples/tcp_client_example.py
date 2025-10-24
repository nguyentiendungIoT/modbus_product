"""
Example: Basic Modbus TCP Client Usage

This example demonstrates how to use the ModbusTCPClient to read and write
registers on a Modbus TCP device.
"""

from modbus_product import ModbusTCPClient


def main():
    # Configure connection parameters
    HOST = "192.168.1.100"  # Change to your device IP
    PORT = 502
    UNIT_ID = 1

    # Create and connect client
    client = ModbusTCPClient(host=HOST, port=PORT, timeout=3.0)

    try:
        # Connect to the device
        print(f"Connecting to {HOST}:{PORT}...")
        client.connect()
        print("Connected successfully!")

        # Read holding registers
        print("\n--- Reading Holding Registers ---")
        registers = client.read_holding_registers(address=0, count=10, unit=UNIT_ID)
        print(f"Registers 0-9: {registers}")

        # Read a single register
        single_register = client.read_holding_registers(
            address=0, count=1, unit=UNIT_ID
        )
        print(f"Register 0: {single_register[0]}")

        # Write a single register
        print("\n--- Writing Single Register ---")
        client.write_register(address=0, value=123, unit=UNIT_ID)
        print("Wrote value 123 to register 0")

        # Verify the write
        result = client.read_holding_registers(address=0, count=1, unit=UNIT_ID)
        print(f"Verified value: {result[0]}")

        # Write multiple registers
        print("\n--- Writing Multiple Registers ---")
        values = [100, 200, 300, 400, 500]
        client.write_registers(address=10, values=values, unit=UNIT_ID)
        print(f"Wrote values {values} to registers 10-14")

        # Read back the written values
        result = client.read_holding_registers(address=10, count=5, unit=UNIT_ID)
        print(f"Verified values: {result}")

        # Read input registers
        print("\n--- Reading Input Registers ---")
        input_regs = client.read_input_registers(address=0, count=5, unit=UNIT_ID)
        print(f"Input registers 0-4: {input_regs}")

    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Always close the connection
        client.close()
        print("\nConnection closed")


if __name__ == "__main__":
    main()
