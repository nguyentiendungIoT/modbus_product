# Getting Started with Modbus Product

This guide will help you get started with the Modbus Product library.

## Installation

### Prerequisites

- Python 3.7 or higher
- pip package manager

### Install from Source

```bash
git clone https://github.com/nguyentiendungIoT/modbus_product.git
cd modbus_product
pip install -e .
```

## Basic Usage

### Modbus TCP

```python
from modbus_product import ModbusTCPClient

# Create client
client = ModbusTCPClient(host='192.168.1.100', port=502)

# Connect
client.connect()

# Read registers
registers = client.read_holding_registers(address=0, count=10, unit=1)
print(f"Registers: {registers}")

# Write register
client.write_register(address=0, value=123, unit=1)

# Close connection
client.close()
```

### Modbus RTU

```python
from modbus_product import ModbusRTUClient

# Create client
client = ModbusRTUClient(port='/dev/ttyUSB0', baudrate=9600)

# Connect
client.connect()

# Read registers
registers = client.read_holding_registers(address=0, count=10, unit=1)
print(f"Registers: {registers}")

# Close connection
client.close()
```

### Using Context Manager

The recommended way to use the clients is with a context manager, which automatically handles connection and cleanup:

```python
from modbus_product import ModbusTCPClient

with ModbusTCPClient(host='192.168.1.100', port=502) as client:
    registers = client.read_holding_registers(address=0, count=10, unit=1)
    print(f"Registers: {registers}")
# Connection automatically closed
```

## Common Operations

### Reading Data

#### Read Holding Registers
```python
registers = client.read_holding_registers(address=0, count=10, unit=1)
```

#### Read Input Registers
```python
registers = client.read_input_registers(address=0, count=10, unit=1)
```

#### Read Coils
```python
coils = client.read_coils(address=0, count=10, unit=1)
```

#### Read Discrete Inputs
```python
inputs = client.read_discrete_inputs(address=0, count=10, unit=1)
```

### Writing Data

#### Write Single Register
```python
client.write_register(address=0, value=123, unit=1)
```

#### Write Multiple Registers
```python
values = [100, 200, 300, 400, 500]
client.write_registers(address=0, values=values, unit=1)
```

#### Write Single Coil
```python
client.write_coil(address=0, value=True, unit=1)
```

#### Write Multiple Coils
```python
values = [True, False, True, True, False]
client.write_coils(address=0, values=values, unit=1)
```

## Working with Different Data Types

### Float Values

```python
from modbus_product.utils import float_to_registers, registers_to_float

# Write float
temperature = 23.456
registers = float_to_registers(temperature)
client.write_registers(address=100, values=registers, unit=1)

# Read float
read_registers = client.read_holding_registers(address=100, count=2, unit=1)
temperature = registers_to_float(read_registers)
```

### Large Integers

```python
from modbus_product.utils import int_to_bytes, bytes_to_int

# Write 32-bit integer
value = 123456
registers = int_to_bytes(value, register_count=2)
client.write_registers(address=200, values=registers, unit=1)

# Read 32-bit integer
read_registers = client.read_holding_registers(address=200, count=2, unit=1)
value = bytes_to_int(read_registers)
```

## Error Handling

```python
from modbus_product import ModbusTCPClient
from modbus_product.exceptions import (
    ModbusConnectionError,
    ModbusReadError,
    ModbusWriteError,
)

try:
    with ModbusTCPClient(host='192.168.1.100', port=502) as client:
        registers = client.read_holding_registers(address=0, count=10, unit=1)
except ModbusConnectionError as e:
    print(f"Connection failed: {e}")
except ModbusReadError as e:
    print(f"Read failed: {e}")
except ModbusWriteError as e:
    print(f"Write failed: {e}")
```

## Configuration

### TCP Client Options

```python
client = ModbusTCPClient(
    host='192.168.1.100',  # Device IP address
    port=502,              # Modbus TCP port (default: 502)
    timeout=3.0            # Operation timeout in seconds (default: 3.0)
)
```

### RTU Client Options

```python
client = ModbusRTUClient(
    port='/dev/ttyUSB0',   # Serial port
    baudrate=9600,         # Baud rate (default: 9600)
    bytesize=8,            # Data bits (default: 8)
    parity='N',            # Parity: 'N', 'E', 'O' (default: 'N')
    stopbits=1,            # Stop bits (default: 1)
    timeout=3.0            # Operation timeout (default: 3.0)
)
```

## Next Steps

- Check out the [API Reference](api_reference.md) for detailed documentation
- Browse the [examples/](../examples/) directory for more examples
- Read the [Contributing Guidelines](../CONTRIBUTING.md) if you want to contribute

## Troubleshooting

### Connection Issues

1. **TCP**: Verify the device IP and port are correct
2. **RTU**: Check the serial port name and permissions
3. **Firewall**: Ensure firewall allows Modbus TCP connections (port 502)
4. **Device**: Confirm the device is powered on and accessible

### Read/Write Errors

1. Verify the address and unit ID are correct
2. Check that you have permission to read/write the registers
3. Ensure the register count doesn't exceed device limits
4. Increase the timeout if operations are timing out

### Serial Port Permission (Linux)

```bash
sudo usermod -a -G dialout $USER
# Logout and login again
```

## Support

For issues and questions:
- GitHub Issues: https://github.com/nguyentiendungIoT/modbus_product/issues
- Email: support@modbusproduct.com
