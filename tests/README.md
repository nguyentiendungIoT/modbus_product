# Modbus Product Tests

This directory contains unit tests for the Modbus Product library.

## Running Tests

### Run all tests:
```bash
pytest
```

### Run with coverage:
```bash
pytest --cov=modbus_product --cov-report=html
```

### Run specific test file:
```bash
pytest tests/test_utils.py
```

### Run specific test class:
```bash
pytest tests/test_utils.py::TestBytesToInt
```

### Run with verbose output:
```bash
pytest -v
```

## Test Structure

- `test_constants.py` - Tests for constants module
- `test_exceptions.py` - Tests for custom exceptions
- `test_utils.py` - Tests for utility functions
- `conftest.py` - Test fixtures and configuration

## Coverage

To view coverage report after running tests with coverage:
```bash
open htmlcov/index.html  # On macOS
xdg-open htmlcov/index.html  # On Linux
start htmlcov/index.html  # On Windows
```
