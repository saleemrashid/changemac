#include <android/log.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LOG_TAG "changemac"

#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)

/* https://android.googlesource.com/kernel/msm/+/android-8.0.0_r0.4/Documentation/devicetree/bindings/wcnss/wcnss-wlan.txt#65 */
#ifndef WCNSS_DEVICE_ADDRESS
#define WCNSS_DEVICE_ADDRESS "fb000000"
#endif
#ifndef WCNSS_DEVICE_COMPATIBLE
#define WCNSS_DEVICE_COMPATIBLE "qcom,wcnss-wlan"
#endif

#ifndef WCNSS_SYSFS_PATH
#define WCNSS_SYSFS_PATH "/sys/bus/platform/devices/" WCNSS_DEVICE_ADDRESS "." WCNSS_DEVICE_COMPATIBLE
#endif

#ifndef WCNSS_MAC_ADDRESS_FILE
#define WCNSS_MAC_ADDRESS_FILE WCNSS_SYSFS_PATH "/wcnss_mac_addr"
#endif

#define MAC_OUI_LENGTH    3
#define MAC_CUSTOM_LENGTH 3

static const uint8_t VENDORS[][MAC_OUI_LENGTH] = {
    { 0x00, 0x26, 0x37 }, /* SAMSUNG ELECTRO MECHANICS CO., LTD. */
    { 0x08, 0x00, 0x28 }, /* Texas Instruments */
    { 0x44, 0x74, 0x6c }, /* Sony Mobile Communications AB */
    { 0x64, 0x16, 0xf0 }, /* HUAWEI TECHNOLOGIES CO.,LTD */
    { 0x80, 0x19, 0x34 }, /* Intel Corporate */
    { 0x84, 0x10, 0x0d }, /* Motorola Mobility LLC, a Lenovo Company */
    { 0x84, 0x3a, 0x4b }, /* Intel Corporate */
    { 0xb0, 0xc0, 0x90 }, /* Chicony Electronics Co., Ltd. */
    { 0xe4, 0xb3, 0x18 }, /* Intel Corporate */
    { 0xf0, 0x25, 0xb7 }, /* SAMSUNG ELECTRO-MECHANICS(THAILAND) */
    { 0xf8, 0xa9, 0xd0 }, /* LG Electronics (Mobile Communications) */
};

#define VENDORS_COUNT (sizeof(VENDORS) / sizeof(*VENDORS))

static bool random_buffer(void *buffer, size_t size);
static uint32_t random_uniform(uint32_t n);
static const uint8_t *random_vendor(void);

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    const uint8_t *vendor = random_vendor();

    uint8_t custom[MAC_CUSTOM_LENGTH];
    random_buffer(custom, sizeof(custom));

    char mac_address[3 * (MAC_OUI_LENGTH + MAC_CUSTOM_LENGTH)];
    snprintf(mac_address, sizeof(mac_address), "%02x:%02x:%02x:%02x:%02x:%02x",
	    vendor[0], vendor[1], vendor[2],
	    custom[0], custom[1], custom[2]);

    ALOGI("Generated MAC address: %s", mac_address);

    FILE *output = fopen(WCNSS_MAC_ADDRESS_FILE, "w");
    if (output == NULL) {
	/* There's nothing we can do about it, the MAC address will probably be leaked */
	ALOGE("Could not open output file: %d", errno);
	return 1;
    }

    if (fputs(mac_address, output) < 0) {
	ALOGE("Could not write to output file: %d", errno);

	if (fclose(output) != 0) {
	    ALOGE("Could not close output file: %d", errno);
	}

	return 1;
    }

    if (fclose(output) != 0) {
	ALOGE("Could not close output file: %d", errno);
	return 1;
    }

    return 0;
}

bool random_buffer(void *buffer, size_t size) {
    static FILE *urandom = NULL;
    if (urandom == NULL) {
	urandom = fopen("/dev/urandom", "r");

	if (urandom == NULL) {
	    /* This should never happen, probably due to SELinux */
	    ALOGE("Could not open /dev/urandom: %d", errno);
	    return false;
	}
    }

    return (fread(buffer, size, 1, urandom) == 1);
}

uint32_t random_uniform(uint32_t n) {
    uint32_t max = UINT32_MAX - (UINT32_MAX % n);

    uint32_t x;
    do {
	if (!random_buffer(&x, sizeof(x))) {
	    ALOGE("Could not read random bytes from stream: %d", errno);

	    /* Ignore the error to avoid leaking the MAC address */
	    srand(time(NULL));
	    return rand() % n;
	}
    } while (x >= max);

    return (x / (max / n));
}

const uint8_t *random_vendor(void) {
    uint32_t x = random_uniform(VENDORS_COUNT);

    ALOGI("Selected vendor index: %" PRIu32 "\n", x);
    return VENDORS[x];
}
