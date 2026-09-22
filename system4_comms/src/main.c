#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include "../drivers/comms/comms.h"

#define COMMS_NODE DT_NODELABEL(comms0)

int main(void)
{
	const struct device *comms = DEVICE_DT_GET(COMMS_NODE);

	if (!device_is_ready(comms)) {
		printk("Comms device not ready\n");
		return 0;
	}

	printk("Waiting for external data (0-255)...\n");

	while (1) {
		uint8_t byte;

		if (comms_receive_byte(comms, &byte) == 0) {
			printk("Received value: %u\n", byte);
		}
		k_msleep(10);
	}

	return 0;
}
