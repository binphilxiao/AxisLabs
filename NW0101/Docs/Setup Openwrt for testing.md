```
vim /etc/opkg/distfeeds.conf
```

add the lines below:

```
#src/gz openwrt_core http://downloads.openwrt.org/releases/18.06-SNAPSHOT/targets/ramips/mt76x8/packages
#src/gz openwrt_base http://downloads.openwrt.org/releases/18.06-SNAPSHOT/packages/mipsel_24kc/base
#src/gz openwrt_luci http://downloads.openwrt.org/releases/18.06-SNAPSHOT/packages/mipsel_24kc/luci
#src/gz openwrt_onion http://downloads.openwrt.org/releases/18.06-SNAPSHOT/packages/mipsel_24kc/onion
#src/gz openwrt_packages http://downloads.openwrt.org/releases/18.06-SNAPSHOT/packages/mipsel_24kc/packages
#src/gz openwrt_routing http://downloads.openwrt.org/releases/18.06-SNAPSHOT/packages/mipsel_24kc/routing
#src/gz openwrt_telephony http://downloads.openwrt.org/releases/18.06-SNAPSHOT/packages/mipsel_24kc/telephony
src/gz omega2_core http://repo.onioniot.com/omega2/packages/core
src/gz omega2_base http://repo.onioniot.com/omega2/packages/base
src/gz omega2_packages http://repo.onioniot.com/omega2/packages/packages
src/gz omega2_routing http://repo.onioniot.com/omega2/packages/routing
src/gz omega2_onion http://repo.onioniot.com/omega2/packages/onion
```

```
vim /etc/opkg.conf
```

comment the lines below:

```
#option check_signature 1
```

```
opkg update
opkg install screen omega2-ctrl gpioctl-sysfs
```

