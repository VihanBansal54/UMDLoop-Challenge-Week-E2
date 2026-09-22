#define DT_DRV_COMPAT custom_rgb_led

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include "rgb_led.h"

/* ---- Config struct: from devicetree, read-only, one per board ---- */
struct rgb_led_config {
	struct gpio_dt_spec red;
	struct gpio_dt_spec green;
	struct gpio_dt_spec blue;
};

/* ---- Data struct: runtime state, mutable, one per instance ---- */
struct rgb_led_data {
	uint32_t last_hex;
};

/* ---- API struct: the function table applications call through ---- */
struct rgb_led_api {
	int (*set_from_hex)(const struct device *dev, uint32_t hex);
};

/*
 * This is the teammate's original decision logic, unchanged in spirit:
 * turn everything off, then light whichever channel is strictly the
 * largest byte of the 24-bit hex value.
 */
static int rgb_led_impl_set_from_hex(const struct device *dev, uint32_t hex)
{
	const struct rgb_led_config *config = dev->config;
	struct rgb_led_data *data = dev->data;

	int r = (hex >> 16) & 0xFF;
	int g = (hex >> 8) & 0xFF;
	int b = hex & 0xFF;

	gpio_pin_set_dt(&config->red, 0);
	gpio_pin_set_dt(&config->green, 0);
	gpio_pin_set_dt(&config->blue, 0);

	if (r > g && r > b) {
		gpio_pin_set_dt(&config->red, 1);
	} else if (g > r && g > b) {
		gpio_pin_set_dt(&config->green, 1);
	} else {
		gpio_pin_set_dt(&config->blue, 1);
	}

	data->last_hex = hex;
	return 0;
}

static const struct rgb_led_api rgb_led_api_funcs = {
	.set_from_hex = rgb_led_impl_set_from_hex,
};

/* Public wrapper: this is what application code (main.c) actually calls */
int rgb_led_set_from_hex(const struct device *dev, uint32_t hex)
{
	const struct rgb_led_api *api = (const struct rgb_led_api *)dev->api;

	return api->set_from_hex(dev, hex);
}

/* ---- Init function: runs once at boot, configures the GPIO pins ---- */
static int rgb_led_init(const struct device *dev)
{
	const struct rgb_led_config *config = dev->config;

	if (!gpio_is_ready_dt(&config->red) ||
	    !gpio_is_ready_dt(&config->green) ||
	    !gpio_is_ready_dt(&config->blue)) {
		return -ENODEV;
	}

	gpio_pin_configure_dt(&config->red, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&config->green, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&config->blue, GPIO_OUTPUT_INACTIVE);

	return 0;
}

/* ---- Instantiation: one block per devicetree node with status "okay" ---- */
#define RGB_LED_INIT(inst)                                                   \
	static struct rgb_led_data rgb_led_data_##inst;                      \
	static const struct rgb_led_config rgb_led_config_##inst = {         \
		.red = GPIO_DT_SPEC_INST_GET(inst, red_gpios),                \
		.green = GPIO_DT_SPEC_INST_GET(inst, green_gpios),            \
		.blue = GPIO_DT_SPEC_INST_GET(inst, blue_gpios),              \
	};                                                                    \
	DEVICE_DT_INST_DEFINE(inst, rgb_led_init, NULL,                      \
			       &rgb_led_data_##inst, &rgb_led_config_##inst,  \
			       POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE, \
			       &rgb_led_api_funcs);

DT_INST_FOREACH_STATUS_OKAY(RGB_LED_INIT)
