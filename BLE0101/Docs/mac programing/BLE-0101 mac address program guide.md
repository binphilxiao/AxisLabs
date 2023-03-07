#### Hardware set up:

#### Download nrf-command-line-tools from Nordic:

https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download

#### Run 'cmd' on windows

![](.\img\run.jpg)

#### Erase UICR registers:

```
nrfjprog --eraseuicr
```

![](.\img\erase.jpg)

#### Program MAC address by following commands:

```
nrfjprog --memwr 0x10001080 -f nrf52 --val 0xF021BCDE
nrfjprog --memwr 0x10001084 -f nrf52 --val 0xFFFFD016
# the MAC address D0:16:F0:21:BC:DE will be programmed, unused byte will be filled as 0XFF
```

![](.\img\program.jpg)

#### Check MAC address in BLE-0101 test firmware:

```
add
```

![](.\img\check.jpg)