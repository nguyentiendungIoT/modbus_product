# Modbus Product Examples

This directory contains example scripts demonstrating how to use the Modbus Product library.

## Examples

### TCP Client Examples

- **tcp_client_example.py** - Basic usage of ModbusTCPClient for reading and writing registers
- **tcp_context_manager.py** - Using context manager for automatic connection handling

### RTU Client Examples

- **rtu_client_example.py** - Basic usage of ModbusRTUClient for serial communication

### Advanced Examples

- **float_example.py** - Working with floating-point values using utility functions

## Running Examples

Before running examples, make sure to:

1. Install the package:
   ```bash
   pip install -e .
   ```

2. Update connection parameters in the example files:
   - For TCP: Change `HOST` and `PORT`
   - For RTU: Change `PORT` and `BAUDRATE`

3. Run an example:
   ```bash
   python examples/tcp_client_example.py
   ```

## Notes

- All examples include error handling
- Remember to close connections when done
- Use context managers (`with` statement) for automatic cleanup
- Adjust unit IDs and addresses based on your device configuration
