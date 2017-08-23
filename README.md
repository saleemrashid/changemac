# changemac

MAC address randomizer for the Qualcomm WCNSS Platform Driver.

## Requirements

 * [Android NDK][1]
 * Qualcomm device with WCNSS
 * Custom recovery or rooted ROM for installation
 * Android 7.0 or above for [`/system/etc/init` support][2]
 * Custom ROM with `/system/addon.d` support for OTA survival

## Working Devices

Please open an issue or a pull request for your device if it works, or doesn't!

### MSM8916

Use `install-msm8916.sh` for these devices:

 * Moto G Play (`harpia`) with LineageOS 14.1

## Installation

In order to use the installation script, ADB must be running as root and
`/system` must be mounted read-write. You can skip these steps if you are
installing from recovery:

```bash
adb root
adb wait-for-device remount
```

For most devices, the device address is [`fb000000`][3], so you can just use
`install.sh` to build and install the files.

Some devices, however, may use different device addresses for the WCNSS device.
For example, MSM8916 devices use [`a000000`][4] and you must use
`install-msm8916.sh` instead of `install.sh`.

If your device uses a different address, please create a copy of
`install-msm8916.sh` for your device and open a pull request.

## Usage

After installation, reboot the device to load the init script.

To check that it is working:

```
adb shell cat /sys/class/net/wlan0/address
adb shell svc wifi disable
adb shell svc wifi enable
adb shell cat /sys/class/net/wlan0/address
```

If it is not working, you can check `logcat` for errors:

```
adb logcat -s changemac
```

[1]: https://developer.android.com/ndk/downloads/index.html
[2]: https://android.googlesource.com/platform/system/core/+/b8dd0272e5decb066152e9dc99bcb41e60264a99
[3]: https://android.googlesource.com/kernel/msm/+/android-8.0.0_r0.4/Documentation/devicetree/bindings/wcnss/wcnss-wlan.txt#65
[4]: https://github.com/MotorolaMobilityLLC/kernel-msm/blob/763cf445be23e3faeb881d23723e53aafa137d23/arch/arm/boot/dts/qcom/msm8916.dtsi#L1105
