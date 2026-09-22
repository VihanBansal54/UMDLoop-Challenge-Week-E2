#define DT_DRV_COMPAT custom_comms

#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include "comms.h"

struct comms_config {
	const struct device *uart;
};

struct comms_data {
	uint8_t last_byte;
};

struct comms_api {
	int (*receive_byte)(const struct device *dev, uint8_t *out_byte);
};

static int comms_impl_receive_byte(const struct device *dev, uint8_t *out_byte)
{
	const struct comms_config *config = dev->config;
	struct comms_data *data = dev->data;
	unsigned char c;

	int rc = uart_poll_in(config->uart, &c);

	if (rc == 0) {
		*out_byte = (uint8_t)c;
		data->last_byte = *out_byte;
	}
	return rc;
}

static const struct comms_api comms_api_funcs = {
	.receive_byte = comms_impl_receive_byte,
};

int comms_receive_byte(const struct device *dev, uint8_t *out_byte)
{
	const struct comms_api *api = (const struct comms_api *)dev->api;

	return api->receive_byte(dev, out_byte);
}

static int comms_init(const struct device *dev)
{
	const struct comms_config *config = dev->config;

	if (!device_is_ready(config->uart)) {
		return -ENODEV;
	}
	return 0;
}

#define COMMS_INIT(inst)                                                    \
	static struct comms_data comms_data_##inst;                         \
	static const struct comms_config comms_config_##inst = {            \
		.uart = DEVICE_DT_GET(DT_INST_PHANDLE(inst, uart)),          \
	};                                                                    \
	DEVICE_DT_INST_DEFINE(inst, comms_init, NULL,                       \
			       &comms_data_##inst, &comms_config_##inst,     \
			       POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE, \
			       &comms_api_funcs);

DT_INST_FOREACH_STATUS_OKAY(COMMS_INIT)
