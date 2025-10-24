"""Test configuration and fixtures."""

import pytest


@pytest.fixture
def mock_modbus_client():
    """Fixture for mocking Modbus client."""

    class MockModbusClient:
        def __init__(self):
            self.connected = False
            self.registers = {i: i * 10 for i in range(100)}

        def connect(self):
            self.connected = True
            return True

        def close(self):
            self.connected = False

        def read_holding_registers(self, address, count, slave):
            class Result:
                def __init__(self, regs):
                    self.registers = regs
                    self.error = False

                def isError(self):
                    return self.error

            return Result([self.registers.get(address + i, 0) for i in range(count)])

    return MockModbusClient()
