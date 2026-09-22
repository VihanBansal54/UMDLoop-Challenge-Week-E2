#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/console/console.h>
#include <stdlib.h>
#include "../drivers/rgb_led/rgb_led.h"

#define RGB_LED_NODE DT_NODELABEL(rgb_led0)

int main(void)
{
	const struct device *rgb_led = DEVICE_DT_GET(RGB_LED_NODE);

	if (!device_is_ready(rgb_led)) {
		printk("RGB LED device not ready\n");
		return 0;
	}

	console_getline_init();
	printk("Enter a hex color (e.g. FF0000) and press enter:\n");

	while (1) {
		char *line = console_getline();
		uint32_t hex = (uint32_t)strtoul(line, NULL, 16);

		rgb_led_set_from_hex(rgb_led, hex);
		printk("Set color from hex: %06X\n", hex);
	}

	return 0;
}
