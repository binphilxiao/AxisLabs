# Quick Start Guide

Follow these steps to build, flash, and run the Radio Test application.

## 1. Build the Application
Open a terminal in the project root and run:

```bash
west build -b <your_board_name>
```
*Example:* `west build -b nrf52840dk_nrf52840`

## 2. Flash the Board
Connect your development kit via USB and run:

```bash
west flash
```

## 3. Connect to Console (RTT)
Since UART is disabled, you must use SEGGER RTT to communicate with the shell.

1.  Open **J-Link RTT Viewer**.
2.  Configure the connection:
    *   **Connection:** USB
    *   **Target Device:** Select your specific Nordic SoC (e.g., `NRF52840_XXAA`).
    *   **Target Interface:** SWD
    *   **Speed:** 4000 kHz (or auto)
    *   **RTT Control Block:** Auto Detection
3.  Click **OK** to connect.

## 4. Basic Usage Examples

Once connected, you should see the shell prompt. If not, try pressing Enter.

### Example 1: Start a Constant Carrier
Transmit an unmodulated carrier on channel 0 at 0 dBm.

```bash
output_power pos0dBm
start_channel 0
start_tx_carrier
```
*To stop:*
```bash
cancel
```

### Example 2: Receive Packets
Set up the device to receive 100 packets on channel 2 using BLE 1Mbit mode.

```bash
data_rate ble_1Mbit
start_channel 2
start_rx 100
```

### Example 3: Modulated TX with Duty Cycle
Transmit modulated packets on channel 10 with 50% duty cycle.

```bash
start_channel 10
start_duty_cycle_modulated_tx 50
```
