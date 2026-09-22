#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include "../drivers/tone/tone.h"

#define TONE_NODE DT_NODELABEL(tone0)

int main(void)
{
	const struct device *tone = DEVICE_DT_GET(TONE_NODE);

	if (!device_is_ready(tone)) {
		printk("Tone device not ready\n");
		return 0;
	}

	printk("Reading ADC and playing a tone for a scaled duration...\n");

	while (1) {
		tone_play(tone);
		k_msleep(1000);
	}

	return 0;
}
