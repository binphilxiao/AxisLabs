| Nordic PIN number | Name        | Direction    | Active | Note                                                      |
| ----------------- | ----------- | ------------ | ------ | --------------------------------------------------------- |
| P0.0              | XTAL1       |              |        |                                                           |
| P0.1              | XTAL2       |              |        |                                                           |
| P0.2              | Vref_8214   | Output       | High   | Set Reverence voltage for DRV8214                         |
| P0.3              | Thermal     | Analog Input |        |                                                           |
| P0.4              | Vbat_div    | Analog Input |        |                                                           |
| P0.5              | Vref        | Analog Input |        | 2.5V 0.5% reference voltage input                         |
| P0.6              | SW_THEM     | Output       | High   | switch for thermal circuirty.                             |
| P0.7              | nBACK_DRV_N | Input        | Low    | It is low when the motor is backdrived reversely manually |
| P0.8              | LED_R_PWM   | Output       | High   | Reserved for LED_RED on control box                       |
| P0.9              | nBTN_RESET  | Input        | Low    | Reset button                                              |
| P0.10             | nBACK_DRV_P | Input        | Low    | It is low when the motor is backdrived manually           |
| P0.11             | nSPI_CS1    | Output       | Low    | SPI bus chip select 1                                     |
| P0.12             | nBTN_USR    | Input        | Low    | User's button                                             |
| P0.13             | LED_G_PWM   | Output       | High   | Reserved for LED_GREEN on control box                     |
| P0.14             | LED_G_PWM   | Output       | High   | Reserved for LED_BLUE on control box                      |
| P0.15             | IN1_8214    | Output       | High   | Enable of DRV8214                                         |
| P0.16             | VDD_MSP_SW  | Output       | High   | Reserved                                                  |
| P0.17             | nSPI_CS0    | Output       | Low    | SPI bus chip select 0                                     |
| P0.18             | nRESET      | Input        | Low    | System reset                                              |
| P0.19             | nSPI_CLK    | Output       |        | SPI bus clock                                             |
| P0.20             | nSPI_D0     | Bi           |        | SPI bus data0                                             |
| P0.21             | nSPI_D1     | Bi           |        | SPI bus data1                                             |
| P0.22             | nSPI_D2     | Bi           |        | SPI bus data2                                             |
| P0.23             | nSPI_D3     | Bi           |        | SPI bus data3                                             |
| P0.24             | nFAULT_8214 | Input        | Low    | fault interrupt of DRV8214                                |
| P0.25             |             |              |        |                                                           |
| P0.26             | SDA         |              |        | IIC bus                                                   |
| P0.27             | SCL         |              |        | IIC bus                                                   |
| P0.28             |             |              |        |                                                           |
| P0.29             |             |              |        |                                                           |
| P0.30             |             |              |        |                                                           |
| P0.31             |             |              |        |                                                           |
|                   |             |              |        |                                                           |
|                   |             |              |        |                                                           |
|                   |             |              |        |                                                           |

| Nordic PIN number | Name        | Direction | Active | Note                                         |
| ----------------- | ----------- | --------- | ------ | -------------------------------------------- |
| P1.0              |             |           |        |                                              |
| P1.1              | IN2_8214    |           |        |                                              |
| P1.2              | VDD_ECD_SW  |           |        | Reserved                                     |
| P1.3              | RC_8214     | Input     |        | Reversed, Configure it as input if not used. |
| P1.4              | V_BAT_SW    |           |        |                                              |
| P1.5              | MFP2        |           |        | Multi functional Pin 2                       |
| P1.6              | MFP1        |           |        | Multi functional Pin 1                       |
| P1.7              | LED_SW      |           |        | Reserved                                     |
| P1.8              |             |           |        |                                              |
| P1.9              | MOTOR_LED_B | Output    | High   | LED_BLUE on motor box                        |
| P1.10             | MOTOR_LED_G | Output    | High   | LED_GREEN on motor box                       |
| P1.11             | MOTOR_LED_R | Output    | High   | LED_RED on motor box                         |
| P1.12             | nSLEEP_8214 | Output    | Low    | Set DRV8214 to sleep                         |
| P1.13             | HES_SEL     |           |        | Reserved                                     |
| P1.14             | INTA        |           |        | Reserved                                     |
| P1.15             | INTB        |           |        | Reserved                                     |
|                   |             |           |        |                                              |
|                   |             |           |        |                                              |
|                   |             |           |        |                                              |

