#!/bin/sh
set -e

ndk-build

adb push libs/armeabi-v7a/changemac /system/xbin/changemac
adb shell chown root:root /system/xbin/changemac
adb shell chmod 755 /system/xbin/changemac
adb shell chcon u:object_r:wcnss_service_exec:s0 /system/xbin/changemac

adb push addon.d/52-changemac.sh /system/addon.d/52-changemac.sh
adb shell chown root:root /system/addon.d/52-changemac.sh
adb shell chmod 755 /system/addon.d/52-changemac.sh
adb shell chcon u:object_r:system_file:s0 /system/addon.d/52-changemac.sh

adb push init/changemac.rc /system/etc/init/changemac.rc
adb shell chown root:root /system/etc/init/changemac.rc
adb shell chmod 644 /system/etc/init/changemac.rc
adb shell chcon u:object_r:system_file:s0 /system/etc/init/changemac.rc
