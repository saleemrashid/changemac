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

## Broken Devices

### MSM8916

 * Moto G 2015 (`osprey`) with LineageOS 14.1

   Even though the `osprey` kernel is built from the same source as `harpia`,
   differences in stack alignment cause the [broken `scanf`][3] to smash the
   stack, triggering a kernel panic. This issue will no doubt affect a number
   of other devices as the `scanf` is used in all WCNSS devices.

## Installation

In order to use the installation script, ADB must be running as root and
`/system` must be mounted read-write. You can skip these steps if you are
installing from recovery:

```bash
adb root
adb wait-for-device remount
```

For most devices, the device address is [`fb000000`][4], so you can just use
`install.sh` to build and install the files.

Some devices, however, may use different device addresses for the WCNSS device.
For example, MSM8916 devices use [`a000000`][5] and you must use
`install-msm8916.sh` instead of `install.sh`.

If your device uses a different address, please create a copy of
`install-msm8916.sh` for your device and open a pull request.

## Usage

After installation, reboot the device to load the init script.

To check that it is working:

```
adb logcat -d -s changemac
adb shell cat /sys/class/net/wlan0/address
```

[1]: https://developer.android.com/ndk/downloads/index.html
[2]: https://android.googlesource.com/platform/system/core/+/b8dd0272e5decb066152e9dc99bcb41e60264a99
[3]: https://github.com/LineageOS/android_kernel_motorola_msm8916/blob/1552c72993a09f34801b479b9873394b2e0f6005/drivers/net/wireless/wcnss/wcnss_wlan.c#L440-L443
[4]: https://android.googlesource.com/kernel/msm/+/android-8.0.0_r0.4/Documentation/devicetree/bindings/wcnss/wcnss-wlan.txt#65
[5]: https://github.com/MotorolaMobilityLLC/kernel-msm/blob/763cf445be23e3faeb881d23723e53aafa137d23/arch/arm/boot/dts/qcom/msm8916.dtsi#L1105
