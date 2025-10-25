# Modbus Commands Reference - Quick Start

## Communication Settings
- **Protocol**: Modbus RTU
- **Slave ID**: 7
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Parity**: None
- **Stop Bits**: 1

---

## Common Modbus Commands (Hex Format)

### 1. X-Axis Forward (1000 pulses)

#### Step 1: Set Pulse Count
```
Command:  07 10 00 00 00 01 02 03 E8
          |  |  |     |  |  |  |  +-- Value: 1000 (0x03E8)
          |  |  |     |  |  |  +-- Byte count: 2
          |  |  |     |  |  +-- Register count: 1
          |  |  |     +-------- Starting address: 0x0000
          |  +---------------- Function code: 16 (Write Multiple Registers)
          +------------------- Slave ID: 7
```

**Calculate CRC and append** (replace CRC with actual):
```
07 10 00 00 00 01 02 03 E8 [CRC_LO] [CRC_HI]
```

#### Step 2: Enable X Forward
```
Command:  07 05 00 04 FF 00
          |  |  |     |  +-- OFF value: 0x0000
          |  |  |     +---- ON value: 0xFF00
          |  |  +---------- Coil address: 4 (COIL_X_POS)
          |  +------------- Function code: 5 (Write Single Coil)
          +---------------- Slave ID: 7
```

**Add CRC**:
```
07 05 00 04 FF 00 [CRC_LO] [CRC_HI]
```

Motor will rotate for 1000 pulses, then stop automatically.

---

### 2. Y-Axis Up (Continuous Mode)

#### Command: Enable Y Forward
```
07 05 00 05 FF 00 [CRC]
   |  |  |     |
   |  |  |     +-- Coil 5 (COIL_Y_POS = up)
   |  +---------- Write single coil
   +------------- Slave ID 7
```

Motor runs continuously until disabled:
```
07 05 00 05 00 00 [CRC]  // Disable to stop
```

---

### 3. Z-Axis Down (500 pulses)

#### Step 1: Set Pulse Count
```
07 10 00 04 00 01 02 01 F4 [CRC]
                      |  +-- Value: 500 (0x01F4)
                      +---- Register 0x04 (Z axis pulses)
```

#### Step 2: Enable Z Backward
```
07 05 00 08 FF 00 [CRC]
                  +-- Coil 8 (COIL_Z_NEG = down)
```

Motor rotates backward for 500 pulses, then stops.

---

### 4. All Axes Forward (Pulse Mode: X=1000, Y=1500, Z=500)

#### Write All Pulse Counts (Multi-Register Write)
```
07 10 00 00 00 03 06 03 E8 05 DC 01 F4 [CRC]
   |  |  |     |  |  |  |  |  |  |  |  +-- Z: 500 (0x01F4)
   |  |  |     |  |  |  |  |  |  +------ Y: 1500 (0x05DC)
   |  |  |     |  |  |  |  +----------- X: 1000 (0x03E8)
   |  |  |     |  |  +----------------- Byte count: 6 (3 registers × 2)
   |  |  |     |  +------------------- Register count: 3
   |  |  |     +---------------------- Starting addr: 0x0000
   +---------------------------------- Function code: 16
```

#### Enable All Motors
```
07 05 00 04 FF 00 [CRC]  // X forward
07 05 00 05 FF 00 [CRC]  // Y forward  
07 05 00 07 FF 00 [CRC]  // Z forward
```

Or use coil write multiple:
```
07 0F 00 03 00 06 01 38 [CRC]
   |  |  |  |  |  |  +-- Byte count: 1
   |  |  |  |  +-------- Coil count: 6
   |  |  |  +---------- Starting coil: 3 (COIL_X_NEG)
   |  +--------------- Function code: 15 (Write Multiple Coils)
   +------------------ Slave ID: 7
   
Coil values: 0x38 = 0011 1000
   Coil 3 (X_NEG):  0
   Coil 4 (X_POS):  1 ✓
   Coil 5 (Y_POS):  1 ✓
   Coil 6 (Y_NEG):  0
   Coil 7 (Z_POS):  1 ✓
   Coil 8 (Z_NEG):  0
```

---

### 5. Read Encoder Pulse Counts

```
07 03 00 00 00 03 [CRC]
   |  |  |  |  |  +-- Quantity: 3 registers
   |  |  |  +-------- Starting address: 0x0000
   |  +------------- Function code: 3 (Read Holding Registers)
   +---------------- Slave ID: 7
```

**Response** (example):
```
07 03 06 03 E8 05 DC 01 F4 [CRC]
   |  |  |  +-- Register 3: 1000 (X pulses)
   |  |  |  +-- Register 2-3: 1500 (Y pulses)  
   |  |  |  +-- Register 4-5: 500 (Z pulses)
   |  |  +---- Byte count: 6
   |  +------- Function code: 3
   +---------- Slave ID: 7
```

---

### 6. Read Coil Status (All Direction Coils)

```
07 01 00 03 00 06 [CRC]
   |  |  |  |  |  +-- Quantity: 6 coils
   |  |  |  +-------- Starting coil: 3 (COIL_X_NEG)
   |  +------------- Function code: 1 (Read Coil Status)
   +---------------- Slave ID: 7
```

**Response** (example, X forward only):
```
07 01 01 10 [CRC]
   |  |  |  +-- Byte count: 1
   |  |  +---- Coil byte: 0x10 = 0001 0000
   |  +------- Function code: 1
   +---------- Slave ID: 7

Bit interpretation:
   Bit 0 (Coil 3): 0 = X_NEG off
   Bit 1 (Coil 4): 1 = X_POS on ✓
   Bit 2 (Coil 5): 0 = Y_POS off
   Bit 3 (Coil 6): 0 = Y_NEG off
   Bit 4 (Coil 7): 0 = Z_POS off
   Bit 5 (Coil 8): 0 = Z_NEG off
```

---

### 7. Stop All Motors

```
07 05 00 04 00 00 [CRC]  // X
07 05 00 05 00 00 [CRC]  // Y
07 05 00 07 00 00 [CRC]  // Z
```

Or disable both direction coils:
```
07 0F 00 03 00 06 01 00 [CRC]  // All coils OFF
```

---

## CRC-16 Calculation (Python)

```python
def compute_crc16(data_bytes):
    """Modbus CRC-16/CCITT-FALSE"""
    crc = 0xFFFF
    for byte in data_bytes:
        crc ^= byte
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    # Return as little-endian bytes
    return bytes([crc & 0xFF, (crc >> 8) & 0xFF])

# Example
frame = bytes.fromhex("070500 04FF00")
crc = compute_crc16(frame)
complete = frame + crc
print(complete.hex().upper())
# Output: 070500 04FF00 [CRC_LO] [CRC_HI]
```

---

## Coil Address Reference

| Coil | Address | Function | Motor |
|------|---------|----------|-------|
| COIL_X_NEG | 3 | X backward (left) | X-Axis |
| COIL_X_POS | 4 | X forward (right) | X-Axis |
| COIL_Y_POS | 5 | Y forward (up) | Y-Axis |
| COIL_Y_NEG | 6 | Y backward (down) | Y-Axis |
| COIL_Z_POS | 7 | Z forward (up) | Z-Axis |
| COIL_Z_NEG | 8 | Z backward (down) | Z-Axis |

---

## Register Address Reference

| Register | Address | Function | Range |
|----------|---------|----------|-------|
| X Pulse Count | 0x0000 | Sets number of pulses for X | 0 - 65535 |
| Y Pulse Count | 0x0002 | Sets number of pulses for Y | 0 - 65535 |
| Z Pulse Count | 0x0004 | Sets number of pulses for Z | 0 - 65535 |

---

## Common Error Responses

| Error | Meaning | Solution |
|-------|---------|----------|
| No response | CRC error or wrong slave ID | Recalculate CRC, check slave ID=7 |
| Exception 03 | Invalid data value | Check register address or value range |
| Exception 02 | Invalid data address | Use valid coil (3-8) or register (0,2,4) |
| Device not responding | No UART connection | Check serial port, baud rate, connections |

---

## Quick Test Sequence

1. **Verify connection**:
   ```
   07 01 00 03 00 06 [CRC]  // Read coils, should get response
   ```

2. **Test X-axis pulse**:
   ```
   07 10 00 00 00 01 02 00 64 [CRC]  // Set 100 pulses
   07 05 00 04 FF 00 [CRC]           // X forward
   [Wait 2 seconds]
   07 03 00 00 00 01 [CRC]           // Read X pulse count
   ```

3. **Test Y-axis continuous**:
   ```
   07 05 00 05 FF 00 [CRC]  // Y forward
   [Wait 2 seconds]
   07 05 00 05 00 00 [CRC]  // Y stop
   ```

4. **Test all axes**:
   ```
   07 0F 00 03 00 06 01 38 [CRC]  // Enable X_POS, Y_POS, Z_POS
   [Wait 5 seconds]
   07 0F 00 03 00 06 01 00 [CRC]  // Disable all
   ```

---

## Testing with Online Tools

1. **CRC Calculator**: https://crccalc.com/
   - Input mode: Hex
   - Algorithm: CRC-16/CCITT-FALSE
   - Paste hex frame without CRC, get CRC bytes

2. **Modbus Tools**:
   - Modbus Poll (Windows GUI)
   - pymodbus (Python library)
   - mbpoll (Linux command-line)
   - Arduino Modbus libraries

---

