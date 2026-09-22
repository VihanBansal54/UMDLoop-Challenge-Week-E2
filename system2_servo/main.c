#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include "../drivers/servo/servo.h"

#define SERVO_NODE DT_NODELABEL(servo0)

int main(void)
{
	const struct device *servo = DEVICE_DT_GET(SERVO_NODE);

	if (!device_is_ready(servo)) {
		printk("Servo device not ready\n");
		return 0;
	}

	printk("Sweeping servo 0 -> 180 -> 0\n");

	for (int pos = 0; pos <= 180; pos++) {
		servo_set_degree(servo, pos);
		k_msleep(8);
	}
	for (int pos = 180; pos >= 0; pos--) {
		servo_set_degree(servo, pos);
		k_msleep(8);
	}

	printk("Sweep complete\n");

	while (1) {
		k_sleep(K_FOREVER);
	}
	return 0;
}
