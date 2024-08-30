####  How to connect power and uart to GR-0203 PCBA

如图所示连接电源于串口线，电源电压9V-12.4V

<img src=".\IMG\1.jpg" style="zoom:50%;" />

串口线连接线序

<img src=".\img\1-2.JPG" style="zoom:50%;" />

注意白色扁平线缆连接方向，接反会损坏设备。

<img src=".\img\1-3.JPG" style="zoom:50%;" />

#### Launch a terminal on PC, baud rate 115200

![](.\IMG\UART_CONFIG.jpg)



#### Print the current settings:

```
parameters_print
```

![](.\IMG\2.jpg)

#### Set output power:

```
output_power pos4dBm
```

Note: Available power setting are below in nrf52840_bitfields.h: 

```
#define RADIO_TXPOWER_TXPOWER_Pos (0UL) /*!< Position of TXPOWER field. */
#define RADIO_TXPOWER_TXPOWER_Msk (0xFFUL << RADIO_TXPOWER_TXPOWER_Pos) /*!< Bit mask of TXPOWER field. */
#define RADIO_TXPOWER_TXPOWER_0dBm (0x0UL) /*!< 0 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos2dBm (0x2UL) /*!< +2 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos3dBm (0x3UL) /*!< +3 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos4dBm (0x4UL) /*!< +4 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos5dBm (0x5UL) /*!< +5 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos6dBm (0x6UL) /*!< +6 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos7dBm (0x7UL) /*!< +7 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos8dBm (0x8UL) /*!< +8 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg40dBm (0xD8UL) /*!< -40 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg30dBm (0xE2UL) /*!< Deprecated enumerator -  -40 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg20dBm (0xECUL) /*!< -20 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg16dBm (0xF0UL) /*!< -16 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg12dBm (0xF4UL) /*!< -12 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg8dBm (0xF8UL) /*!< -8 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg4dBm (0xFCUL) /*!< -4 dBm */
```

#### Select frequency:

```
start_channel fnum
```

```
# The frequency will be   2400 Mhz+fnum Mhz
# For example 2424Mhz, type in 'start_channel 24'
```

![](.\img\3.jpg)

#### Type pattern command:

```
transmit_pattern pattern_11110000
```

#### To start radio Tx, type: 

```
start_duty_cycle_modulated_tx 100
```

![](.\img\4.jpg)

Note: If the duty cycle is too low(like 5%), a lower power will be ovserved. It is recommended to use duty cycle >80%.

#### To stop radio Tx, type:

```
cancel
```

#### To start radio Rx:

```
start_rx
```

#### To stop radio Rx:

```
cancel
```
