"""
Example: Modbus TCP Client with Context Manager

This example demonstrates using the context manager for automatic
connection handling.
"""

from modbus_product import ModbusTCPClient


def main():
    # Configure connection parameters
    HOST = "192.168.1.100"  # Change to your device IP
    PORT = 502
    UNIT_ID = 1

    # Use context manager for automatic connection/disconnection
    print(f"Connecting to {HOST}:{PORT}...")

    try:
        with ModbusTCPClient(host=HOST, port=PORT, timeout=3.0) as client:
            print("Connected successfully!")

            # Read holding registers
            registers = client.read_holding_registers(address=0, count=10, unit=UNIT_ID)
            print(f"Registers: {registers}")

            # Write a register
            client.write_register(address=0, value=999, unit=UNIT_ID)
            print("Register written successfully")

        # Connection automatically closed after exiting context
        print("Connection closed automatically")

    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()
