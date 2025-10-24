# API Reference

Complete API reference for the Modbus Product library.

## ModbusTCPClient

Modbus TCP client for communication over TCP/IP network.

### Constructor

```python
ModbusTCPClient(host: str, port: int = 502, timeout: float = 3.0)
```

**Parameters:**
- `host` (str): IP address or hostname of the Modbus device
- `port` (int, optional): TCP port number (default: 502)
- `timeout` (float, optional): Timeout for operations in seconds (default: 3.0)

**Example:**
```python
client = ModbusTCPClient(host='192.168.1.100', port=502, timeout=5.0)
```

### Methods

#### connect()

Connect to the Modbus TCP device.

```python
def connect() -> bool
```

**Returns:** `True` if connection successful

**Raises:** `ModbusConnectionError` if connection fails

#### close()

Close the connection to the Modbus TCP device.

```python
def close() -> None
```

#### is_connected()

Check if client is connected.

```python
def is_connected() -> bool
```

**Returns:** `True` if connected, `False` otherwise

#### read_holding_registers()

Read holding registers from the device.

```python
def read_holding_registers(address: int, count: int = 1, unit: int = 1) -> List[int]
```

**Parameters:**
- `address` (int): Starting address to read from
- `count` (int, optional): Number of registers to read (default: 1)
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** List of register values

**Raises:** `ModbusReadError` if read operation fails

#### read_input_registers()

Read input registers from the device.

```python
def read_input_registers(address: int, count: int = 1, unit: int = 1) -> List[int]
```

**Parameters:**
- `address` (int): Starting address to read from
- `count` (int, optional): Number of registers to read (default: 1)
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** List of register values

**Raises:** `ModbusReadError` if read operation fails

#### read_coils()

Read coils from the device.

```python
def read_coils(address: int, count: int = 1, unit: int = 1) -> List[bool]
```

**Parameters:**
- `address` (int): Starting address to read from
- `count` (int, optional): Number of coils to read (default: 1)
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** List of coil values (True/False)

**Raises:** `ModbusReadError` if read operation fails

#### read_discrete_inputs()

Read discrete inputs from the device.

```python
def read_discrete_inputs(address: int, count: int = 1, unit: int = 1) -> List[bool]
```

**Parameters:**
- `address` (int): Starting address to read from
- `count` (int, optional): Number of inputs to read (default: 1)
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** List of input values (True/False)

**Raises:** `ModbusReadError` if read operation fails

#### write_register()

Write a single register.

```python
def write_register(address: int, value: int, unit: int = 1) -> bool
```

**Parameters:**
- `address` (int): Address to write to
- `value` (int): Value to write
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** `True` if write successful

**Raises:** `ModbusWriteError` if write operation fails

#### write_registers()

Write multiple registers.

```python
def write_registers(address: int, values: List[int], unit: int = 1) -> bool
```

**Parameters:**
- `address` (int): Starting address to write to
- `values` (List[int]): List of values to write
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** `True` if write successful

**Raises:** `ModbusWriteError` if write operation fails

#### write_coil()

Write a single coil.

```python
def write_coil(address: int, value: bool, unit: int = 1) -> bool
```

**Parameters:**
- `address` (int): Address to write to
- `value` (bool): Value to write (True/False)
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** `True` if write successful

**Raises:** `ModbusWriteError` if write operation fails

#### write_coils()

Write multiple coils.

```python
def write_coils(address: int, values: List[bool], unit: int = 1) -> bool
```

**Parameters:**
- `address` (int): Starting address to write to
- `values` (List[bool]): List of values to write (True/False)
- `unit` (int, optional): Unit ID of the device (default: 1)

**Returns:** `True` if write successful

**Raises:** `ModbusWriteError` if write operation fails

---

## ModbusRTUClient

Modbus RTU client for serial communication.

### Constructor

```python
ModbusRTUClient(
    port: str,
    baudrate: int = 9600,
    bytesize: int = 8,
    parity: str = 'N',
    stopbits: int = 1,
    timeout: float = 3.0
)
```

**Parameters:**
- `port` (str): Serial port name (e.g., '/dev/ttyUSB0' on Linux, 'COM1' on Windows)
- `baudrate` (int, optional): Baud rate (default: 9600)
- `bytesize` (int, optional): Number of data bits (default: 8)
- `parity` (str, optional): Parity setting - 'N' (None), 'E' (Even), 'O' (Odd) (default: 'N')
- `stopbits` (int, optional): Number of stop bits (default: 1)
- `timeout` (float, optional): Timeout for operations in seconds (default: 3.0)

**Example:**
```python
client = ModbusRTUClient(
    port='/dev/ttyUSB0',
    baudrate=19200,
    parity='E',
    timeout=5.0
)
```

### Methods

ModbusRTUClient has the same methods as ModbusTCPClient. See above for details.

---

## Utility Functions

### float_to_registers()

Convert a float to two register values (32-bit IEEE 754).

```python
def float_to_registers(value: float) -> List[int]
```

**Parameters:**
- `value` (float): Float value to convert

**Returns:** List of two register values

**Example:**
```python
from modbus_product.utils import float_to_registers
registers = float_to_registers(23.456)  # [16609, 38010]
```

### registers_to_float()

Convert two register values to a float (32-bit IEEE 754).

```python
def registers_to_float(registers: List[int]) -> float
```

**Parameters:**
- `registers` (List[int]): List of two register values

**Returns:** Float value

**Raises:** `ValueError` if register count is not 2

**Example:**
```python
from modbus_product.utils import registers_to_float
value = registers_to_float([16609, 38010])  # 23.456
```

### int_to_bytes()

Convert an integer to a list of register values.

```python
def int_to_bytes(value: int, register_count: int = 2) -> List[int]
```

**Parameters:**
- `value` (int): Integer value to convert
- `register_count` (int, optional): Number of registers to use (default: 2)

**Returns:** List of register values (16-bit each)

### bytes_to_int()

Convert a list of register values to an integer.

```python
def bytes_to_int(data: List[int], signed: bool = False) -> int
```

**Parameters:**
- `data` (List[int]): List of register values (16-bit each)
- `signed` (bool, optional): Whether to interpret as signed integer (default: False)

**Returns:** Integer value

### validate_address()

Validate a Modbus address.

```python
def validate_address(address: int) -> bool
```

**Parameters:**
- `address` (int): Address to validate

**Returns:** `True` if valid

**Raises:** `ValueError` if address is invalid

### validate_register_value()

Validate a register value.

```python
def validate_register_value(value: int) -> bool
```

**Parameters:**
- `value` (int): Value to validate

**Returns:** `True` if valid

**Raises:** `ValueError` if value is invalid

---

## Exceptions

### ModbusException

Base exception for all Modbus-related errors.

```python
class ModbusException(Exception)
```

### ModbusConnectionError

Exception raised when connection to Modbus device fails.

```python
class ModbusConnectionError(ModbusException)
```

### ModbusReadError

Exception raised when reading from Modbus device fails.

```python
class ModbusReadError(ModbusException)
```

### ModbusWriteError

Exception raised when writing to Modbus device fails.

```python
class ModbusWriteError(ModbusException)
```

### ModbusTimeoutError

Exception raised when Modbus operation times out.

```python
class ModbusTimeoutError(ModbusException)
```

### ModbusInvalidAddressError

Exception raised when an invalid address is provided.

```python
class ModbusInvalidAddressError(ModbusException)
```

### ModbusInvalidValueError

Exception raised when an invalid value is provided.

```python
class ModbusInvalidValueError(ModbusException)
```

---

## Constants

### Function Codes

- `READ_COILS = 0x01`
- `READ_DISCRETE_INPUTS = 0x02`
- `READ_HOLDING_REGISTERS = 0x03`
- `READ_INPUT_REGISTERS = 0x04`
- `WRITE_SINGLE_COIL = 0x05`
- `WRITE_SINGLE_REGISTER = 0x06`
- `WRITE_MULTIPLE_COILS = 0x0F`
- `WRITE_MULTIPLE_REGISTERS = 0x10`

### Default Values

- `DEFAULT_TCP_PORT = 502`
- `DEFAULT_UNIT_ID = 1`
- `DEFAULT_TIMEOUT = 3.0`
- `DEFAULT_BAUDRATE = 9600`
- `DEFAULT_BYTESIZE = 8`
- `DEFAULT_PARITY = "N"`
- `DEFAULT_STOPBITS = 1`

### Limits

- `MAX_REGISTER_COUNT = 125`
- `MAX_COIL_COUNT = 2000`
- `MAX_REGISTER_VALUE = 65535`
- `MAX_ADDRESS = 65535`
