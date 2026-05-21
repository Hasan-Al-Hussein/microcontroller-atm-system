# Microcontroller ATM System

Microprocessor-based ATM simulation implemented on the FRDM-KL25Z development board using keypad input, UART communication, timeout handling, and transaction validation logic.

<p align="center">
  <img src="images/atm_system_block_diagram.png" width="950"/>
</p>

---

# Overview

This project implements a simplified ATM system using the FRDM-KL25Z microcontroller platform.

The system supports:
- PIN authentication
- Keypad-based user input
- UART terminal communication
- Withdrawal amount validation
- Timeout protection
- Transaction state management

The project demonstrates low-level embedded system programming using GPIO, UART peripherals, keypad scanning logic, and finite transaction workflows.

---

# Hardware Interface

<p align="center">
  <img src="images/keypad_interface.png" width="950"/>
</p>

The ATM system integrates:
- FRDM-KL25Z development board
- 4x4 matrix keypad
- UART serial terminal
- Push-button input

The keypad is used for:
- PIN entry
- Withdrawal amount input
- Transaction interaction

---

# ATM System Architecture

<p align="center">
  <img src="images/atm_system_block_diagram.png" width="950"/>
</p>

The system architecture consists of:
- GPIO-based keypad interface
- UART communication module
- PIN verification logic
- Timeout monitoring
- Transaction validation
- User interaction workflow

---

# Keypad Scanning Algorithm

<p align="center">
  <img src="images/keypad_scanning_flowchart.png" width="850"/>
</p>

The keypad scanning mechanism operates using:
- row-column activation
- GPIO polling
- debounce handling
- timeout-based detection

Each row is sequentially driven LOW while column inputs are monitored to identify pressed keys.

---

# Key Detection Logic

<p align="center">
  <img src="images/key_detection_logic.png" width="850"/>
</p>

The keypad matrix maps:
- numeric digits
- function keys
- transaction control inputs

The system translates row-column combinations into corresponding key values.

---

# ATM Transaction Logic

The ATM workflow performs:

1. Button-triggered transaction start
2. PIN entry through keypad
3. PIN verification
4. Withdrawal amount input
5. Balance validation
6. Transaction confirmation

The system includes:
- incorrect PIN handling
- retry limits
- timeout detection
- transaction abort logic

---

# Performance Evaluation

<p align="center">
  <img src="images/test_plan.png" width="950"/>
</p>

The system was tested under multiple transaction scenarios.

| Test Scenario | Expected Result |
|---|---|
| Correct PIN and withdrawal ≤ 5000 | Transaction success |
| Correct PIN and withdrawal > 5000 | Insufficient balance |
| Wrong PIN on first try, correct second | Incorrect Pin → Entered pin is correct |
| Wrong PIN three times | Incorrect Pin. Aborted! |
| No PIN input within timeout | Timeout!! |
| No withdrawal input within timeout | Timeout!! |

---

# Features

- 4x4 keypad scanning
- UART communication
- PIN authentication
- Withdrawal validation
- Timeout protection
- Transaction retry logic
- GPIO peripheral programming
- Embedded finite-state workflow

---

# Source Code

```text
src/
├── atm_main.c
├── keypad_scan.c
├── uart_driver.c
└── README.md
```

The implementation includes:
- GPIO configuration
- UART initialization
- keypad scanning logic
- timeout-based input handling
- transaction validation
- embedded ATM workflow control

---

# Technologies Used

## Embedded Systems

- C
- FRDM-KL25Z
- GPIO Programming
- UART Communication
- Matrix Keypad Scanning

## Embedded Concepts

- Polling
- Debouncing
- Timeout Logic
- State-Based Control
- Peripheral Initialization

---

# Results

| Metric | Result |
|---|---|
| Platform | FRDM-KL25Z |
| Input Method | 4x4 Matrix Keypad |
| Communication | UART |
| PIN Verification | Implemented |
| Timeout Protection | Implemented |
| Transaction Validation | Implemented |

---

# Key Engineering Concepts

- Embedded systems programming
- GPIO interfacing
- UART communication
- Keypad scanning
- Finite-state transaction control
- Timeout handling
- Debouncing
- Peripheral configuration

---

# Documentation

- [Full Technical Report](docs/microprocessor_atm_report.pdf)

---

# Authors

- Hasan Al Hussein

Khalifa University
