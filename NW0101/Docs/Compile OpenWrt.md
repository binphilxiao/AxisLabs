```
# Do not login by root 
git clone https://github.com/OnionIoT/source.git
```

```
cd source
git checkout openwrt-21.02-dev
```

```
make menuconfig
# For `Target System`, select `MediaTek Ralink MIPS`
# For `Subtarget`, select `MT76x8 based boards`
# For `Target Profile`, select `Multiple Devices`
# A new item will appear on the Main menu, `Target Devices`
# For `Target Devices`, select `Onion Omega2` and `Onion Omega2+`
# Exit and save your configuration
```

```
make 
```

