# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2025-10-24

### Added
- Initial release of Modbus Product library
- ModbusTCPClient for Modbus TCP communication
- ModbusRTUClient for Modbus RTU/serial communication
- Support for reading holding registers, input registers, coils, and discrete inputs
- Support for writing single and multiple registers and coils
- Comprehensive error handling with custom exceptions
- Utility functions for data type conversions (float, int)
- Context manager support for automatic connection handling
- Full test suite with pytest
- Complete documentation and examples
- CI/CD pipeline with GitHub Actions
- Type hints throughout the codebase
- Professional project structure

### Features
- Modbus TCP and RTU protocol support
- Reading and writing operations for all register types
- Float and integer conversion utilities
- Address and value validation
- Comprehensive exception hierarchy
- Logging support
- Cross-platform compatibility (Linux, Windows, macOS)
- Python 3.7+ support

[0.1.0]: https://github.com/nguyentiendungIoT/modbus_product/releases/tag/v0.1.0
