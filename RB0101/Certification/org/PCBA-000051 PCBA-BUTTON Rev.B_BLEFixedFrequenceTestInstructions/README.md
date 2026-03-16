# Radio Test Sample

## Overview
This application is designed for testing the radio performance of Nordic Semiconductor SoCs using the Zephyr RTOS. It provides a command-line interface (Shell) to configure radio parameters and execute various test modes such as unmodulated carrier, modulated carrier, RX/TX sweeps, and duty cycle tests.

**Note:** This project is configured to use **SEGGER RTT (Real Time Transfer)** for the console and shell interaction. UART is disabled by default to minimize pin usage and interference.

## Features
- **Radio Modes:** Supports various data rates including 1Mbit, 2Mbit, BLE 1Mbit, BLE 2Mbit, and Long Range (if supported by HW).
- **Test Patterns:** Random, 11110000, 11001100.
- **Output Power:** Configurable TX power levels.
- **Test Modes:**
  - Unmodulated TX Carrier
  - Modulated TX Carrier
  - RX/TX Sweep
  - RX Mode (Packet Counter)
  - Duty Cycle Modulated TX

## Configuration
The project configuration is defined in `prj.conf`.
- **Console:** RTT (`CONFIG_USE_SEGGER_RTT=y`, `CONFIG_RTT_CONSOLE=y`)
- **UART:** Disabled (`CONFIG_SERIAL=n`)

## Shell Commands
The application exposes the following commands via the RTT Shell:

### General Configuration
- `start_channel <channel>`: Set start channel (0-80).
- `end_channel <channel>`: Set end channel for sweep (0-80).
- `time_on_channel <ms>`: Set time on each channel for sweep (1-99 ms).
- `data_rate <rate>`: Set data rate.
  - Subcommands: `nrf_1Mbit`, `nrf_2Mbit`, `ble_1Mbit`, `ble_2Mbit`, `ble_lr125Kbit`, `ble_lr500Kbit`, `ieee802154_250Kbit`.
- `output_power <power>`: Set TX power.
  - Subcommands: `pos4dBm`, `pos0dBm`, `neg4dBm`, etc. (Available options depend on SoC).
- `transmit_pattern <pattern>`: Set transmission pattern.
  - Subcommands: `pattern_random`, `pattern_11110000`, `pattern_11001100`.
- `parameters_print`: Print current configuration.

### Test Execution
- `start_tx_carrier`: Start unmodulated TX carrier.
- `start_tx_modulated_carrier [packets]`: Start modulated TX carrier. Optional packet count.
- `start_duty_cycle_modulated_tx <percent>`: Start modulated TX with duty cycle (1-90%).
- `start_tx_sweep`: Start TX channel sweep.
- `start_rx_sweep`: Start RX channel sweep.
- `start_rx [packets]`: Start RX mode. Optional packet count.
- `print_rx`: Print received packet statistics.
- `cancel`: Stop the current test.

## Hardware Requirements
- Nordic Semiconductor DK (e.g., nRF52840 DK, nRF5340 DK, nRF54L15 DK).
- J-Link Debugger (usually built-in on DKs).

## Software Requirements
- nRF Connect SDK (NCS).
- J-Link Software and Documentation Pack (for RTT Viewer).
