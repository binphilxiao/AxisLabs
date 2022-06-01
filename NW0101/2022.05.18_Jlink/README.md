# 如何使用
下载debug.cfg与ramboot.bin, 其中debug.cfg为OpenOCD配置文件, ramboot.bin为预先编译好的uboot, 代码空间位于内存当中.  
如果使用了gpio模拟jlink，比如使用树莓派或者另一7688等，请使用这个配置文件启动OpenOCD:  

```
openocd -f pi_gpio.cfg
```
之后请telnet连接到openocd的会话，执行 ddrinit 命令初始化DDR内存.  
DDR内存初始化成功后, 执行 load_ramboot 命令, 将uboot下载到ram中运行. 之后您即可使用uboot向flash烧写新的代码.
```
telnet localhost 4444
> reset halt
> ddrinit
> load_image /home/pi/ramboot-cache.bin 0x80200000
> resume 0x80200000
```

# 特殊说明
您也可以自行编译您自己的uboot并使用该脚本下载到ram当中运行, 当然, 您需要自行寻找编译uboot到MT76x8的方法.  
这个项目可以自动识别您的DDR与soc类型, 理论上可通用所有MT76x8系列芯片.  
如果ramboot.bin加载成功但串口仍然无任何输出, 则可能是您的设备完全没有任何固件导致, 如遇该情况您可以尝试使用ramboot-cache.bin代替ramboot.bin加载到设备当中.

# 常用命令
```
reset
halt

mdw 0xB0000000 1				# read CHIPID 0-3, return 3637544d
mdw 0xB0000004 1				# read CHIPID 0-3, return 20203832
mdw 0xB0000028 1				# read ROM_STATUS, return 00000071
mdw 0xB000000c 1				# read CHIP_REV_ID, return 00010102
mdw 0xB0000010 1				# read SYSCFG0, return 00080054
mdw 0xB0000014 1				# read SYSCFG1, return 02605500
mdw 0xB000002c 1				# read CLKCFG0, return 0020100c
mww 0xB000002c 0x0020100c 1 	# write CLKCFG0
mdw 0xB0000030 1				# read CLKCFG1, return f9bfffc0
mww 0xB0000030 0xf9bfffc0 1 	# write CLKCFG1
mdw 0xB0000440 1				# read DYNCFG0, return 00030101
mww 0xB0000440 0x00030a01 1 	# write DYNCFG0

mdw 0xB0000038 1				# read RSTSTAT, return c0030200
mww 0xB0000038 0x00030000 1 	# write RSTSTAT, disable WDT

load_image /home/pi/ramboot.bin 0x80200000
dump_image ramboot_tmp.bin 0x80200000 150000
```



