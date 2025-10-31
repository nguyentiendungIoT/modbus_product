# STM32F407 Modbus Servo Control - Test Cases

## Build Status
✅ **BUILD SUCCESSFUL**
- Compiled: 25,840 bytes
- No critical errors
- Minor warnings: Unused static variables in modbusSlave.h (not impacting functionality)

---

## Test Environment Setup

### Hardware Requirements
- STM32F407VET6 development board with ST-Link debugger
- 3 DC servo motors with encoders (or logic analyzers to observe PWM/direction signals)
- Modbus RTU master (Modbus Tester, PC application, PLC, or Python script)
- RS-485/UART converter (if testing remotely)
- Logic analyzer or oscilloscope (optional, for signal verification)

### Software Requirements
- STM32CubeIDE or similar STM32 debugger
- Modbus RTU test tool (e.g., Modbus Poll, ModbusTester, or Python pymodbus)
- Baud rate: 115200 bps, No parity, 1 stop bit

### Hardware Connections
- USART2 (PA2/PA3): Modbus RTU communications
- Motor Control Pins: PWM on TIM1/TIM8/TIM3, Direction on GPIO

---

## Test Cases

### Category 1: Basic Initialization & Startup

#### TC-1.1: Firmware Flash & Boot
**Objective**: Verify firmware boots correctly after flashing
- **Action**: Flash `test.elf` to STM32F407, power on board
- **Expected**: 
  - No hard faults or crashes
  - USART2 ready (can receive Modbus commands)
  - All timers (TIM1, TIM3, TIM8, TIM2, TIM5, TIM9) initialized
- **Pass Criteria**: Board responsive, LEDs (if available) indicate normal state

#### TC-1.2: Modbus Slave Ready
**Objective**: Verify Modbus RTU slave accepts connections
- **Action**: Connect to USART2, send empty/dummy Modbus packet
- **Expected**: 
  - Device responds or ignores gracefully (slave ID=7)
  - No reset or crash
- **Pass Criteria**: Board remains stable

---

### Category 2: X-Axis Motor Control

#### TC-2.1: X-Axis Continuous Forward (Right)
**Objective**: X motor runs continuously to the right
- **Modbus Command**: Write coil 4 (COIL_X_POS) = ON, coil 3 (COIL_X_NEG) = OFF
- **Expected**: 
  - PWM signal on TIM1_CH1 (PA8) at 10 kHz
  - GPIO DIR1_X asserts high (right direction)
  - Motor rotates continuously until coil is turned OFF
- **Pass Criteria**: Motor spins consistently, no stuttering

#### TC-2.2: X-Axis Continuous Backward (Left)
**Objective**: X motor runs continuously to the left
- **Modbus Command**: Write coil 3 (COIL_X_NEG) = ON, coil 4 (COIL_X_POS) = OFF
- **Expected**: 
  - PWM signal on TIM1_CH1 at 10 kHz
  - GPIO DIR1_X asserts low (left direction)
  - Motor rotates continuously in opposite direction
- **Pass Criteria**: Motor rotates smoothly in reverse

#### TC-2.3: X-Axis Pulse Mode (1000 pulses)
**Objective**: X motor rotates exactly 1000 pulses to the right
- **Modbus Command**: 
  1. Write register at address 0x00 = 1000 (pulse count for X)
  2. Write coil 4 (COIL_X_POS) = ON
- **Expected**: 
  - Motor rotates, encoder counter (TIM2) increments
  - After 1000 pulses, motor stops
  - Final pulse count ≈ 1000
- **Pass Criteria**: Motor stops after ~1000 encoder pulses (±5% tolerance acceptable)

#### TC-2.4: X-Axis Stop
**Objective**: X motor stops cleanly
- **Modbus Command**: Write both coils 3 & 4 = OFF
- **Expected**: 
  - PWM duty cycle goes to 0%
  - Motor decelerates to stop
  - GPIO direction signals remain stable
- **Pass Criteria**: Motor stops smoothly

---

### Category 3: Y-Axis Motor Control

#### TC-3.1: Y-Axis Continuous Up
**Objective**: Y motor runs continuously upward
- **Modbus Command**: Write coil 5 (COIL_Y_POS) = ON, coil 6 (COIL_Y_NEG) = OFF
- **Expected**: 
  - PWM signal on TIM8_CH1 (PC6) at 10 kHz
  - GPIO DIR1_Y asserts high (up direction)
  - Motor rotates continuously
- **Pass Criteria**: Motor spins consistently upward

#### TC-3.2: Y-Axis Continuous Down
**Objective**: Y motor runs continuously downward
- **Modbus Command**: Write coil 6 (COIL_Y_NEG) = ON, coil 5 (COIL_Y_POS) = OFF
- **Expected**: 
  - PWM signal on TIM8_CH1 at 10 kHz
  - GPIO DIR1_Y asserts low (down direction)
  - Motor rotates in opposite direction
- **Pass Criteria**: Motor spins smoothly in reverse

#### TC-3.3: Y-Axis Pulse Mode (500 pulses)
**Objective**: Y motor rotates exactly 500 pulses upward
- **Modbus Command**: 
  1. Write register at address 0x02 = 500 (pulse count for Y)
  2. Write coil 5 (COIL_Y_POS) = ON
- **Expected**: 
  - Motor rotates, encoder counter (TIM5) increments
  - After 500 pulses, motor stops
  - Final pulse count ≈ 500
- **Pass Criteria**: Motor stops after ~500 encoder pulses

#### TC-3.4: Y-Axis Stop
**Objective**: Y motor stops cleanly
- **Modbus Command**: Write both coils 5 & 6 = OFF
- **Expected**: Motor stops smoothly, direction signals stable
- **Pass Criteria**: No oscillation or residual PWM

---

### Category 4: Z-Axis Motor Control

#### TC-4.1: Z-Axis Continuous Up
**Objective**: Z motor runs continuously upward
- **Modbus Command**: Write coil 7 (COIL_Z_POS) = ON, coil 8 (COIL_Z_NEG) = OFF
- **Expected**: 
  - PWM signal on TIM3_CH3 (PB8) at 10 kHz
  - GPIO DIR1_Z asserts high (up direction)
  - Motor rotates continuously
- **Pass Criteria**: Motor spins consistently

#### TC-4.2: Z-Axis Continuous Down
**Objective**: Z motor runs continuously downward
- **Modbus Command**: Write coil 8 (COIL_Z_NEG) = ON, coil 7 (COIL_Z_POS) = OFF
- **Expected**: 
  - PWM signal on TIM3_CH3 at 10 kHz
  - GPIO DIR1_Z asserts low (down direction)
  - Motor rotates in reverse
- **Pass Criteria**: Motor runs smoothly in opposite direction

#### TC-4.3: Z-Axis Pulse Mode (2000 pulses)
**Objective**: Z motor rotates exactly 2000 pulses upward
- **Modbus Command**: 
  1. Write register at address 0x04 = 2000 (pulse count for Z)
  2. Write coil 7 (COIL_Z_POS) = ON
- **Expected**: 
  - Motor rotates, encoder counter (TIM9) increments
  - After 2000 pulses, motor stops
  - Final pulse count ≈ 2000
- **Pass Criteria**: Motor stops after ~2000 encoder pulses (±5%)

#### TC-4.4: Z-Axis Stop
**Objective**: Z motor stops cleanly
- **Modbus Command**: Write both coils 7 & 8 = OFF
- **Expected**: Motor stops smoothly
- **Pass Criteria**: No overshooting or vibration

---

### Category 5: Multi-Axis Control

#### TC-5.1: All Axes Forward Simultaneously
**Objective**: All 3 motors run forward at the same time
- **Modbus Command**: Write coils 4, 5, 7 = ON; write coils 3, 6, 8 = OFF
- **Expected**: 
  - All 3 motors rotate simultaneously
  - All PWM signals at 10 kHz
  - All direction signals correct
  - No interference or cross-talk
- **Pass Criteria**: All motors spin together without issues

#### TC-5.2: All Axes Pulse Mode Simultaneously
**Objective**: All 3 motors rotate simultaneously with different pulse counts
- **Modbus Command**: 
  1. Write register 0x00 = 1000 (X pulses)
  2. Write register 0x02 = 2000 (Y pulses)
  3. Write register 0x04 = 500 (Z pulses)
  4. Write coils 4, 5, 7 = ON
- **Expected**: 
  - X stops after 1000 pulses
  - Y stops after 2000 pulses
  - Z stops after 500 pulses
  - All operate independently without synchronization issues
- **Pass Criteria**: Each axis reaches target pulse count accurately

#### TC-5.3: Sequential Axis Control
**Objective**: Enable/disable axes in sequence
- **Modbus Commands** (in sequence):
  1. X forward only for 1 second
  2. Y up only for 1 second
  3. Z up only for 1 second
  4. All stop
- **Expected**: One motor runs at a time, clean transitions
- **Pass Criteria**: No cross-interference, each motor controls independently

#### TC-5.4: Direction Change Under Load
**Objective**: Reverse motor direction while running
- **Modbus Command Sequence**: 
  1. Enable X forward (continuous mode)
  2. After 2 seconds, disable X forward
  3. Enable X backward
- **Expected**: 
  - Motor smoothly transitions to reverse
  - No sudden jerks or reset
  - Direction GPIO changes correctly
- **Pass Criteria**: Smooth direction reversal

---

### Category 6: Edge Cases & Error Handling

#### TC-6.1: Both Direction Coils ON (Invalid State)
**Objective**: Set both forward and backward coils ON
- **Modbus Command**: Write coils 3 & 4 = ON (X axis)
- **Expected**: 
  - Motor behavior implementation-dependent (may disable PWM, run at reduced power, or run one direction)
  - No crash or reset
- **Pass Criteria**: Device remains stable and recovers

#### TC-6.2: Pulse Count = 0
**Objective**: Set pulse count to 0 with direction enabled
- **Modbus Command**: 
  1. Write register 0x00 = 0
  2. Write coil 4 (X_POS) = ON
- **Expected**: 
  - Motor either doesn't move or moves continuously (depending on implementation)
  - No crash
- **Pass Criteria**: Handled gracefully

#### TC-6.3: Very Large Pulse Count
**Objective**: Set pulse count to maximum value (65535)
- **Modbus Command**: 
  1. Write register 0x00 = 65535
  2. Write coil 4 (X_POS) = ON
- **Expected**: 
  - Motor runs until 65535 pulses are reached
  - May take extended time (graceful degradation)
- **Pass Criteria**: No overflow, counter wraps correctly

#### TC-6.4: Rapid On/Off Toggle
**Objective**: Toggle motor coil rapidly
- **Modbus Command**: Write coil 4 ON, then immediately OFF, repeat 10x
- **Expected**: 
  - Motor responds to each toggle
  - No latching or missed commands
  - Smooth behavior (no excessive acceleration/deceleration)
- **Pass Criteria**: All commands acknowledged, no corruption

#### TC-6.5: Modbus CRC Error Recovery
**Objective**: Send frame with incorrect CRC
- **Modbus Command**: Send valid command but corrupt last 2 bytes (CRC)
- **Expected**: 
  - Device ignores bad frame
  - No error response, command not executed
  - Ready for next valid frame
- **Pass Criteria**: Recovers without crash

#### TC-6.6: Invalid Slave ID
**Objective**: Send Modbus command with wrong slave ID (not 7)
- **Modbus Command**: Send command with slave ID = 5 or any non-7 value
- **Expected**: 
  - Device ignores command (not for this slave)
  - No response transmitted
  - Ready for next frame
- **Pass Criteria**: No spurious responses

#### TC-6.7: UART Buffer Overflow
**Objective**: Send very long Modbus frame or rapid consecutive frames
- **Modbus Command**: Send multiple commands in quick succession
- **Expected**: 
  - Device processes frames in order
  - No buffer overflow crash
  - May drop or delay frames if buffer full (acceptable)
- **Pass Criteria**: Device recovers, no hard fault

---

### Category 7: Communication Protocol

#### TC-7.1: Read Holding Registers (Function Code 3)
**Objective**: Read encoder counter (pulse) values
- **Modbus Command**: Read holding register starting at address 0x00, qty=1
  - Register 0x00: X axis pulse count
  - Register 0x02: Y axis pulse count
  - Register 0x04: Z axis pulse count
- **Expected**: 
  - Valid Modbus response with current pulse counts
  - CRC correct
  - Values reflect encoder state
- **Pass Criteria**: Correct register values returned

#### TC-7.2: Write Single Coil (Function Code 5)
**Objective**: Control single motor direction via coil write
- **Modbus Command**: Write single coil at address 4 (COIL_X_POS) = ON
- **Expected**: 
  - Motor responds
  - Echo response confirms write
- **Pass Criteria**: Motor starts, response valid

#### TC-7.3: Read Coil Status (Function Code 1)
**Objective**: Read current coil states
- **Modbus Command**: Read coil from address 3 (COIL_X_NEG), qty=6 (all direction coils)
- **Expected**: 
  - Response shows current state of all 6 direction coils
  - Values match last write commands
- **Pass Criteria**: Accurate coil status reported

#### TC-7.4: Write Multiple Registers (Function Code 16)
**Objective**: Set pulse counts for multiple axes in one command
- **Modbus Command**: Write registers 0x00, 0x02, 0x04 with values (1000, 1500, 2000)
- **Expected**: 
  - All registers updated in one transaction
  - Response confirms multi-register write
- **Pass Criteria**: All values set correctly

---

## Test Execution Checklist

### Pre-Test
- [ ] STM32F407 board powered and connected
- [ ] ST-Link debugger connected
- [ ] USART2 TX/RX verified on oscilloscope/logic analyzer
- [ ] Motors mechanically free to rotate (no load or light load)
- [ ] Modbus test tool configured (slave ID=7, 115200 baud)

### During Test
- [ ] Each test case run independently
- [ ] Results logged (pass/fail, observations)
- [ ] Motor rotation direction visually confirmed
- [ ] No unexpected resets or watchdog triggers
- [ ] UART traffic captured for anomalies

### Post-Test
- [ ] All test results compiled
- [ ] Summary report generated
- [ ] Known issues documented
- [ ] Recommendations for next revision noted

---

## Expected Outcomes Summary

| Category | Test Count | Critical | Status |
|----------|-----------|----------|--------|
| Initialization | 2 | 2 | To Test |
| X-Axis Control | 4 | 2 | To Test |
| Y-Axis Control | 4 | 2 | To Test |
| Z-Axis Control | 4 | 2 | To Test |
| Multi-Axis | 4 | 2 | To Test |
| Edge Cases | 7 | 4 | To Test |
| Protocol | 4 | 2 | To Test |
| **Total** | **29** | **16** | **Pending** |

---

## Notes for Testing Team

1. **Encoder Pulse Accuracy**: ±5% tolerance acceptable due to motor inertia and encoder resolution
2. **PWM Frequency**: Should be stable at 10 kHz; check with oscilloscope if issues occur
3. **Modbus Timeout**: Default implementation may not have timeout; device always waits for commands
4. **Motor Inertia**: After pulse count reached, motor may coast for 50-200ms before stopping
5. **Direction GPIO Hold Time**: Typically 5-10ms before/after PWM changes
6. **Concurrent Operations**: Device supports all 3 axes simultaneously without synchronization; each axis independent

---

**Test Report Template**: Create `TEST_REPORT.md` with your results, mark each TC as PASS/FAIL with observations.

