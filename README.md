# Modbus Product

[![Python Version](https://img.shields.io/badge/python-3.7%2B-blue.svg)](https://www.python.org/downloads/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Code Style: Black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)

A professional, easy-to-use Python library for Modbus communication in industrial IoT applications. This library provides a clean and intuitive interface for both Modbus TCP and RTU protocols.

## Features

- ✅ **Modbus TCP Client** - Connect to Modbus TCP devices over network
- ✅ **Modbus RTU Client** - Connect to Modbus RTU devices over serial
- ✅ **Simple API** - Easy-to-use interface for reading/writing registers
- ✅ **Type Safety** - Full type hints for better IDE support
- ✅ **Error Handling** - Comprehensive error handling and logging
- ✅ **Well Documented** - Extensive documentation and examples
- ✅ **Production Ready** - Tested and reliable for industrial use

## Installation

### From PyPI (Coming Soon)

```bash
pip install modbus-product
```

### From Source

```bash
git clone https://github.com/nguyentiendungIoT/modbus_product.git
cd modbus_product
pip install -e .
```

## Quick Start

### Modbus TCP Client

```python
from modbus_product import ModbusTCPClient

# Create a client and connect
client = ModbusTCPClient(host='192.168.1.100', port=502)
client.connect()

# Read holding registers
registers = client.read_holding_registers(address=0, count=10, unit=1)
print(f"Registers: {registers}")

# Write a single register
client.write_register(address=0, value=123, unit=1)

# Close connection
client.close()
```

### Modbus RTU Client

```python
from modbus_product import ModbusRTUClient

# Create a client and connect
client = ModbusRTUClient(port='/dev/ttyUSB0', baudrate=9600)
client.connect()

# Read input registers
registers = client.read_input_registers(address=0, count=5, unit=1)
print(f"Input Registers: {registers}")

# Close connection
client.close()
```

### Using Context Manager

```python
from modbus_product import ModbusTCPClient

# Automatically handle connection and cleanup
with ModbusTCPClient(host='192.168.1.100', port=502) as client:
    registers = client.read_holding_registers(address=0, count=10, unit=1)
    print(f"Registers: {registers}")
```

## Documentation

Full documentation is available in the [docs/](docs/) directory:

- [Getting Started Guide](docs/getting_started.md)
- [API Reference](docs/api_reference.md)
- [Examples](examples/)
- [Contributing Guidelines](CONTRIBUTING.md)

## Project Structure

```
modbus_product/
├── modbus_product/          # Main package
│   ├── __init__.py         # Package initialization
│   ├── client.py           # Client base classes
│   ├── tcp_client.py       # Modbus TCP client
│   ├── rtu_client.py       # Modbus RTU client
│   ├── constants.py        # Modbus constants
│   ├── exceptions.py       # Custom exceptions
│   └── utils.py            # Utility functions
├── examples/               # Example scripts
├── tests/                  # Unit tests
├── docs/                   # Documentation
├── setup.py               # Package setup
├── requirements.txt       # Dependencies
├── requirements-dev.txt   # Development dependencies
└── README.md             # This file
```

## Requirements

- Python 3.7 or higher
- pymodbus >= 3.0.0
- pyserial >= 3.5 (for RTU support)

## Development

### Setup Development Environment

```bash
# Clone the repository
git clone https://github.com/nguyentiendungIoT/modbus_product.git
cd modbus_product

# Create virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install development dependencies
pip install -r requirements-dev.txt
pip install -e .
```

### Running Tests

```bash
# Run all tests
pytest

# Run with coverage
pytest --cov=modbus_product --cov-report=html

# Run specific test file
pytest tests/test_tcp_client.py
```

### Code Quality

```bash
# Format code with black
black modbus_product tests

# Check code style with flake8
flake8 modbus_product tests

# Type checking with mypy
mypy modbus_product
```

## Contributing

Contributions are welcome! Please read our [Contributing Guidelines](CONTRIBUTING.md) before submitting a pull request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support

- 📧 Email: support@modbusproduct.com
- 🐛 Issues: [GitHub Issues](https://github.com/nguyentiendungIoT/modbus_product/issues)
- 💬 Discussions: [GitHub Discussions](https://github.com/nguyentiendungIoT/modbus_product/discussions)

## Acknowledgments

- Built with [pymodbus](https://github.com/pymodbus-dev/pymodbus)
- Inspired by the industrial automation community
- Thanks to all contributors!

## Roadmap

- [ ] Add Modbus ASCII support
- [ ] Implement Modbus server functionality
- [ ] Add more comprehensive examples
- [ ] Create GUI for testing Modbus connections
- [ ] Add support for custom function codes
- [ ] Implement data logging and visualization

---

Made with ❤️ for the Industrial IoT Community