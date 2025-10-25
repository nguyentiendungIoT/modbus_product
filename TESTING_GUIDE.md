# STM32F407 Modbus Servo Control - Testing Guide

## Quick Start Testing

### 1. Build & Flash Firmware
```bash
# Build
cd d:\workspace_cubeide_project\test\test
.\build.bat

# Flash (using STM32CubeIDE or command line)
.\flash.bat
```

After successful flash, the board should boot and be ready to receive Modbus commands via USART2 (115200 baud, no parity, 1 stop bit, slave ID=7).

---

## Testing with Modbus Poll (Windows GUI Tool)

### Setup Modbus Poll

1. **Download**: Get "Modbus Poll" from [https://www.modbustools.com/](https://www.modbustools.com/)
2. **Install** and launch
3. **Create New Connection**:
   - Connection → Connect
   - Select **RTU** mode
   - Port: COM (your UART adapter port)
   - Baud: 115200
   - Data bits: 8
   - Parity: None
   - Stop bits: 1
   - Slave ID: 7

### Test Cases in Modbus Poll

#### Test 1: Read Encoder Counters (X Axis)
```
Function: Read Holding Registers (Function 3)
Starting Register: 0x0000 (address 0)
Quantity: 1
Expected: Returns current X-axis encoder count
```

#### Test 2: Write Pulse Count (X Axis Forward 1000 pulses)
```
Function: Write Multiple Registers (Function 16)
Starting Register: 0x0000
Data: 1000 (0x03E8)
```

#### Test 3: Enable X-Axis Forward Motion
```
Function: Write Single Coil (Function 5)
Coil Address: 4 (COIL_X_POS)
Value: ON (0xFF00)
Expected: Motor rotates forward for 1000 pulses, then stops
```

#### Test 4: Read All Direction Coils
```
Function: Read Coil Status (Function 1)
Starting Coil: 3 (COIL_X_NEG)
Quantity: 6 (reads coils 3-8, all direction control)
Expected: Shows state of all 6 direction coils
```

---

## Testing with Python (Automated)

### Setup Python Environment
```bash
# Install pymodbus
pip install pymodbus

# Or using conda
conda install -c conda-forge pymodbus
```

### Python Test Script

Save as `test_modbus.py`:

```python
#!/usr/bin/env python3
"""
STM32F407 Modbus Servo Control - Test Script
Tests all 3 axes with continuous and pulse modes
"""

from pymodbus.client import ModbusSerialClient as ModbusClient
from pymodbus.exceptions import ModbusException
import time
import sys

# Configuration
PORT = "COM3"  # Change to your serial port
SLAVE_ID = 7
BAUDRATE = 115200

# Coil addresses
COIL_X_NEG = 3
COIL_X_POS = 4
COIL_Y_POS = 5
COIL_Y_NEG = 6
COIL_Z_POS = 7
COIL_Z_NEG = 8

# Register addresses for pulse counts
REG_X_PULSES = 0x0000
REG_Y_PULSES = 0x0002
REG_Z_PULSES = 0x0004

def connect_client():
    """Connect to Modbus device"""
    client = ModbusClient(
        method="rtu",
        port=PORT,
        baudrate=BAUDRATE,
        stopbits=1,
        bytesize=8,
        parity='N',
        timeout=1
    )
    
    if not client.connect():
        print(f"❌ Failed to connect to {PORT}")
        sys.exit(1)
    
    print(f"✅ Connected to Modbus slave {SLAVE_ID} on {PORT} @ {BAUDRATE} baud")
    return client

def read_coils(client, start, count):
    """Read coil status"""
    try:
        result = client.read_coils(start, count, slave=SLAVE_ID)
        if result.isError():
            print(f"❌ Error reading coils: {result}")
            return None
        return result.bits
    except Exception as e:
        print(f"❌ Exception reading coils: {e}")
        return None

def write_coil(client, addr, state):
    """Write single coil"""
    try:
        result = client.write_coil(addr, state, slave=SLAVE_ID)
        if result.isError():
            print(f"❌ Error writing coil {addr}: {result}")
            return False
        return True
    except Exception as e:
        print(f"❌ Exception writing coil {addr}: {e}")
        return False

def read_registers(client, start, count):
    """Read holding registers"""
    try:
        result = client.read_holding_registers(start, count, slave=SLAVE_ID)
        if result.isError():
            print(f"❌ Error reading registers: {result}")
            return None
        return result.registers
    except Exception as e:
        print(f"❌ Exception reading registers: {e}")
        return None

def write_register(client, addr, value):
    """Write holding register"""
    try:
        result = client.write_register(addr, value, slave=SLAVE_ID)
        if result.isError():
            print(f"❌ Error writing register {addr}: {result}")
            return False
        return True
    except Exception as e:
        print(f"❌ Exception writing register {addr}: {e}")
        return False

def test_x_continuous():
    """Test X-axis continuous forward"""
    print("\n🔧 TEST: X-Axis Continuous Forward")
    client = connect_client()
    
    print("  Setting X to continuous forward...")
    write_coil(client, COIL_X_NEG, False)
    write_coil(client, COIL_X_POS, True)
    
    print("  Motor should run forward. Press ENTER to stop...")
    input()
    
    print("  Stopping X...")
    write_coil(client, COIL_X_POS, False)
    print("✅ Test complete\n")
    client.close()

def test_x_pulse():
    """Test X-axis pulse mode (1000 pulses)"""
    print("\n🔧 TEST: X-Axis Pulse Mode (1000 pulses)")
    client = connect_client()
    
    print("  Setting pulse count to 1000...")
    write_register(client, REG_X_PULSES, 1000)
    
    print("  Reading back pulse count...")
    pulses = read_registers(client, REG_X_PULSES, 1)
    if pulses:
        print(f"  Current pulse count: {pulses[0]}")
    
    print("  Enabling X forward (will rotate 1000 pulses)...")
    write_coil(client, COIL_X_NEG, False)
    write_coil(client, COIL_X_POS, True)
    
    print("  Waiting for motor to complete...")
    time.sleep(5)  # Adjust based on motor speed
    
    print("  Reading final pulse count...")
    final_pulses = read_registers(client, REG_X_PULSES, 1)
    if final_pulses:
        print(f"  ✓ Final pulse count: {final_pulses[0]}")
    
    # Stop
    write_coil(client, COIL_X_POS, False)
    print("✅ Test complete\n")
    client.close()

def test_all_axes_continuous():
    """Test all 3 axes simultaneously (continuous mode)"""
    print("\n🔧 TEST: All Axes Continuous Forward")
    client = connect_client()
    
    print("  Starting all motors forward...")
    write_coil(client, COIL_X_NEG, False)
    write_coil(client, COIL_X_POS, True)
    write_coil(client, COIL_Y_NEG, False)
    write_coil(client, COIL_Y_POS, True)
    write_coil(client, COIL_Z_NEG, False)
    write_coil(client, COIL_Z_POS, True)
    
    print("  All motors should be running. Press ENTER to stop...")
    input()
    
    print("  Stopping all motors...")
    write_coil(client, COIL_X_POS, False)
    write_coil(client, COIL_Y_POS, False)
    write_coil(client, COIL_Z_POS, False)
    print("✅ Test complete\n")
    client.close()

def test_all_axes_pulse():
    """Test all 3 axes with different pulse counts"""
    print("\n🔧 TEST: All Axes Pulse Mode")
    print("  X: 1000 pulses, Y: 1500 pulses, Z: 500 pulses")
    
    client = connect_client()
    
    print("  Setting pulse counts...")
    write_register(client, REG_X_PULSES, 1000)
    write_register(client, REG_Y_PULSES, 1500)
    write_register(client, REG_Z_PULSES, 500)
    
    print("  Starting all motors...")
    write_coil(client, COIL_X_POS, True)
    write_coil(client, COIL_Y_POS, True)
    write_coil(client, COIL_Z_POS, True)
    
    print("  Waiting for all motors to complete...")
    time.sleep(10)  # Adjust based on motor speed
    
    print("  Reading final pulse counts...")
    x_final = read_registers(client, REG_X_PULSES, 1)
    y_final = read_registers(client, REG_Y_PULSES, 1)
    z_final = read_registers(client, REG_Z_PULSES, 1)
    
    if x_final and y_final and z_final:
        print(f"  ✓ X: {x_final[0]} pulses")
        print(f"  ✓ Y: {y_final[0]} pulses")
        print(f"  ✓ Z: {z_final[0]} pulses")
    
    print("✅ Test complete\n")
    client.close()

def test_read_all_coils():
    """Read all direction coils status"""
    print("\n🔧 TEST: Read All Direction Coils")
    client = connect_client()
    
    coils = read_coils(client, COIL_X_NEG, 6)
    if coils:
        print(f"  COIL_X_NEG (3):  {coils[0]}")
        print(f"  COIL_X_POS (4):  {coils[1]}")
        print(f"  COIL_Y_POS (5):  {coils[2]}")
        print(f"  COIL_Y_NEG (6):  {coils[3]}")
        print(f"  COIL_Z_POS (7):  {coils[4]}")
        print(f"  COIL_Z_NEG (8):  {coils[5]}")
    
    print("✅ Test complete\n")
    client.close()

def main():
    """Main test menu"""
    tests = {
        '1': ("X-Axis Continuous", test_x_continuous),
        '2': ("X-Axis Pulse Mode", test_x_pulse),
        '3': ("All Axes Continuous", test_all_axes_continuous),
        '4': ("All Axes Pulse Mode", test_all_axes_pulse),
        '5': ("Read All Coils", test_read_all_coils),
    }
    
    print("=" * 60)
    print("STM32F407 Modbus Servo Control - Test Suite")
    print("=" * 60)
    
    while True:
        print("\nSelect test:")
        for key, (name, _) in tests.items():
            print(f"  {key}: {name}")
        print("  q: Quit")
        
        choice = input("\nEnter choice: ").strip()
        
        if choice == 'q':
            print("Exiting...")
            break
        elif choice in tests:
            try:
                tests[choice][1]()
            except KeyboardInterrupt:
                print("\n⚠️  Test interrupted by user")
            except Exception as e:
                print(f"❌ Error during test: {e}")
        else:
            print("❌ Invalid choice")

if __name__ == "__main__":
    main()
```

### Run Python Tests
```bash
# Edit PORT variable in script to match your serial port
python test_modbus.py
```

---

## Testing with Serial Terminal (Hex Mode)

If using a generic serial terminal like Putty or Arduino Serial Monitor:

### Manual Modbus Frame Construction

**Example: Enable X-Axis Forward (Coil 4 ON)**

Modbus RTU frame structure:
```
Slave ID | Function Code | Address (2 bytes) | ON/OFF | CRC (2 bytes)
07       | 05            | 00 04             | FF 00  | CRC_LO CRC_HI
```

Complete hex frame: `07 05 00 04 FF 00 [CRC]`

Use online CRC calculator or Python to calculate CRC:
```python
from pymodbus.utilities import computeCRC

frame = bytes.fromhex("070500 04FF00")
crc = computeCRC(frame)
complete_frame = frame + crc
print(complete_frame.hex())
# Output: 070500 04FF00 [CRC bytes]
```

Send as raw hex via terminal.

---

## Troubleshooting

### Issue: Device not responding
- [ ] Check USART2 connections (PA2, PA3)
- [ ] Verify baud rate is 115200
- [ ] Confirm slave ID = 7
- [ ] Check CRC calculation
- [ ] Verify serial port is correct

### Issue: Motor doesn't rotate
- [ ] Check PWM signal with oscilloscope (should be 10 kHz)
- [ ] Verify GPIO direction pins are toggling
- [ ] Ensure motor has power supply
- [ ] Check motor is mechanically free

### Issue: Pulse count not accurate
- [ ] Motor inertia may cause overshoot (±5% tolerance)
- [ ] Verify encoder is connected and working
- [ ] Check for encoder glitches on oscilloscope
- [ ] Reduce pulse count for testing

### Issue: Watchdog reset or crash
- [ ] Check Modbus frame validity (CRC, length)
- [ ] Verify no buffer overflow (send frames with delays)
- [ ] Check for stack overflow (increase stack size if needed)
- [ ] Monitor UART for corruption

---

## Performance Metrics to Verify

| Metric | Expected | Test Method |
|--------|----------|-------------|
| PWM Frequency | 10 kHz | Oscilloscope on PWM pin |
| PWM Duty Cycle | 0-100% | Oscilloscope measurement |
| Pulse Accuracy | ±5% | Compare encoder count vs command |
| Direction GPIO Timing | <10ms from coil write | Logic analyzer |
| Response Time | <10ms | Modbus capture tool |
| Modbus Baud | 115200 | Serial analyzer |

---

## Log & Report

After testing, document:
1. **Test Date/Time**
2. **Hardware Used** (Motors, encoders, serial adapter)
3. **Firmware Version** (test.elf date/size)
4. **Results Summary** (# passed, # failed)
5. **Issues Found** (with severity: Critical/Major/Minor)
6. **Recommendations** for next build

Example template:
```markdown
# Test Report - 2025-10-25

## Environment
- Board: STM32F407VET6
- Firmware: test.elf (25,840 bytes)
- Modbus Tool: Modbus Poll 4.0
- Motors: 3x DC servo with encoders

## Results
- TC-2.1 (X Continuous): PASS
- TC-2.3 (X Pulse 1000): PASS ✓ (Actual: 998 pulses, within tolerance)
- TC-5.2 (All Axes Pulse): PASS ✓

## Issues
- None critical

## Recommendations
- Consider tighter encoder tolerance if higher accuracy needed
```

---

