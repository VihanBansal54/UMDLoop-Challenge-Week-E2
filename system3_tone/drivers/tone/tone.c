#define DT_DRV_COMPAT custom_tone

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include "tone.h"

struct tone_config {
	struct pwm_dt_spec pwm;
	struct adc_dt_spec adc;
};

struct tone_data {
	struct adc_sequence sequence;
	int16_t sample_buffer;
};

struct tone_api {
	int (*play)(const struct device *dev);
};

static int tone_impl_play(const struct device *dev)
{
	const struct tone_config *config = dev->config;
	struct tone_data *data = dev->data;

	int rc = adc_read_dt(&config->adc, &data->sequence);
	if (rc != 0) {
		return rc;
	}

	int32_t raw = data->sample_buffer;
	if (raw < 0) {
		raw = 0;
	}

	/* Map the 12-bit ADC reading (0-4095) to a 0-2000 ms duration */
	uint32_t duration_ms = ((uint32_t)raw * 2000) / 4095;

	/* 50% duty cycle produces an audible tone at the PWM's frequency */
	pwm_set_pulse_dt(&config->pwm, config->pwm.period / 2);
	k_msleep(duration_ms);
	pwm_set_pulse_dt(&config->pwm, 0);

	return 0;
}

static const struct tone_api tone_api_funcs = {
	.play = tone_impl_play,
};

int tone_play(const struct device *dev)
{
	const struct tone_api *api = (const struct tone_api *)dev->api;

	return api->play(dev);
}

static int tone_init(const struct device *dev)
{
	const struct tone_config *config = dev->config;
	struct tone_data *data = dev->data;
	int rc;

	if (!pwm_is_ready_dt(&config->pwm)) {
		return -ENODEV;
	}

	if (!adc_is_ready_dt(&config->adc)) {
		return -ENODEV;
	}

	rc = adc_channel_setup_dt(&config->adc);
	if (rc != 0) {
		return rc;
	}

	rc = adc_sequence_init_dt(&config->adc, &data->sequence);
	if (rc != 0) {
		return rc;
	}

	data->sequence.buffer = &data->sample_buffer;
	data->sequence.buffer_size = sizeof(data->sample_buffer);

	return 0;
}

#define TONE_INIT(inst)                                                     \
	static struct tone_data tone_data_##inst;                           \
	static const struct tone_config tone_config_##inst = {              \
		.pwm = PWM_DT_SPEC_INST_GET(inst),                           \
		.adc = ADC_DT_SPEC_GET_BY_IDX(DT_DRV_INST(inst), 0),         \
	};                                                                    \
	DEVICE_DT_INST_DEFINE(inst, tone_init, NULL,                        \
			       &tone_data_##inst, &tone_config_##inst,       \
			       POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE, \
			       &tone_api_funcs);

DT_INST_FOREACH_STATUS_OKAY(TONE_INIT)
