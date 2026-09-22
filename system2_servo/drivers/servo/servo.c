#define DT_DRV_COMPAT custom_servo

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include "servo.h"

struct servo_config {
	struct pwm_dt_spec pwm;
	uint32_t min_pulse_us;
	uint32_t max_pulse_us;
};

struct servo_data {
	uint8_t current_degree;
};

struct servo_api {
	int (*set_degree)(const struct device *dev, uint8_t degree);
};

static int servo_impl_set_degree(const struct device *dev, uint8_t degree)
{
	const struct servo_config *config = dev->config;
	struct servo_data *data = dev->data;

	if (degree > 180) {
		degree = 180;
	}

	uint32_t pulse_us = config->min_pulse_us +
		((config->max_pulse_us - config->min_pulse_us) * degree) / 180;

	int rc = pwm_set_pulse_dt(&config->pwm, PWM_USEC(pulse_us));

	if (rc == 0) {
		data->current_degree = degree;
	}
	return rc;
}

static const struct servo_api servo_api_funcs = {
	.set_degree = servo_impl_set_degree,
};

int servo_set_degree(const struct device *dev, uint8_t degree)
{
	const struct servo_api *api = (const struct servo_api *)dev->api;

	return api->set_degree(dev, degree);
}

static int servo_init(const struct device *dev)
{
	const struct servo_config *config = dev->config;

	if (!pwm_is_ready_dt(&config->pwm)) {
		return -ENODEV;
	}
	return 0;
}

#define SERVO_INIT(inst)                                                    \
	static struct servo_data servo_data_##inst;                         \
	static const struct servo_config servo_config_##inst = {            \
		.pwm = PWM_DT_SPEC_INST_GET(inst),                           \
		.min_pulse_us = DT_INST_PROP_OR(inst, min_pulse_us, 500),    \
		.max_pulse_us = DT_INST_PROP_OR(inst, max_pulse_us, 2400),   \
	};                                                                    \
	DEVICE_DT_INST_DEFINE(inst, servo_init, NULL,                       \
			       &servo_data_##inst, &servo_config_##inst,     \
			       POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE, \
			       &servo_api_funcs);

DT_INST_FOREACH_STATUS_OKAY(SERVO_INIT)
