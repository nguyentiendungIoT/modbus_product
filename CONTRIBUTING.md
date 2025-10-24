# Contributing to Modbus Product

Thank you for your interest in contributing to Modbus Product! This document provides guidelines and instructions for contributing.

## Code of Conduct

- Be respectful and inclusive
- Welcome newcomers and help them learn
- Focus on constructive feedback
- Maintain professionalism

## How to Contribute

### Reporting Issues

1. Check if the issue already exists in the [issue tracker](https://github.com/nguyentiendungIoT/modbus_product/issues)
2. If not, create a new issue with:
   - Clear title and description
   - Steps to reproduce (for bugs)
   - Expected vs actual behavior
   - Your environment (OS, Python version, etc.)

### Suggesting Enhancements

1. Check existing issues and discussions
2. Create a new issue with the "enhancement" label
3. Describe the enhancement and its benefits
4. Include examples if applicable

### Pull Requests

1. Fork the repository
2. Create a new branch for your feature/fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Make your changes following the code style guidelines
4. Write/update tests for your changes
5. Ensure all tests pass
6. Update documentation if needed
7. Commit your changes with clear messages
8. Push to your fork
9. Open a pull request

## Development Setup

### Clone the Repository

```bash
git clone https://github.com/nguyentiendungIoT/modbus_product.git
cd modbus_product
```

### Create Virtual Environment

```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

### Install Development Dependencies

```bash
pip install -r requirements-dev.txt
pip install -e .
```

## Code Style Guidelines

### Python Style

We follow [PEP 8](https://pep8.org/) with some modifications:

- Line length: 88 characters (Black default)
- Use type hints for function signatures
- Write docstrings for all public functions and classes
- Use meaningful variable and function names

### Formatting

We use [Black](https://github.com/psf/black) for code formatting:

```bash
black modbus_product tests
```

### Import Sorting

We use [isort](https://pycqa.github.io/isort/) for import sorting:

```bash
isort modbus_product tests
```

### Linting

We use [flake8](https://flake8.pycqa.org/) for linting:

```bash
flake8 modbus_product tests
```

### Type Checking

We use [mypy](http://mypy-lang.org/) for type checking:

```bash
mypy modbus_product
```

## Testing

### Running Tests

Run all tests:
```bash
pytest
```

Run with coverage:
```bash
pytest --cov=modbus_product --cov-report=html
```

Run specific test:
```bash
pytest tests/test_utils.py::TestBytesToInt::test_single_register
```

### Writing Tests

- Write tests for all new features
- Maintain or improve code coverage
- Use descriptive test names
- Follow the existing test structure
- Use pytest fixtures when appropriate

Example test:
```python
def test_read_holding_registers():
    """Test reading holding registers."""
    client = ModbusTCPClient(host='localhost', port=502)
    with pytest.raises(ModbusConnectionError):
        client.read_holding_registers(0, 10, 1)
```

## Documentation

### Docstring Format

Use Google-style docstrings:

```python
def function_name(param1: int, param2: str) -> bool:
    """
    Brief description of function.

    Longer description if needed.

    Args:
        param1: Description of param1
        param2: Description of param2

    Returns:
        Description of return value

    Raises:
        ValueError: Description of when this is raised
    """
    pass
```

### Updating Documentation

- Update README.md for user-facing changes
- Update docs/ for API changes
- Add examples for new features
- Keep documentation concise and clear

## Commit Messages

Write clear, concise commit messages:

```
Add feature to read multiple register types

- Implemented batch reading for efficiency
- Added tests for new functionality
- Updated documentation
```

Format:
- Use present tense ("Add feature" not "Added feature")
- First line: brief summary (50 chars or less)
- Blank line
- Detailed description if needed
- Reference issues: "Fixes #123"

## Release Process

1. Update version in `setup.py` and `__init__.py`
2. Update CHANGELOG.md
3. Create a git tag: `git tag -a v0.2.0 -m "Release v0.2.0"`
4. Push tag: `git push origin v0.2.0`
5. Create GitHub release
6. Publish to PyPI (maintainers only)

## Questions?

- Open a discussion on GitHub
- Ask in an issue
- Email: support@modbusproduct.com

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Recognition

Contributors will be added to the CONTRIBUTORS.md file and mentioned in release notes.

Thank you for contributing to Modbus Product! 🎉
