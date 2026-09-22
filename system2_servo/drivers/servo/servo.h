#ifndef SERVO_H_
#define SERVO_H_

#include <zephyr/device.h>
#include <stdint.h>

int servo_set_degree(const struct device *dev, uint8_t degree);

#endif /* SERVO_H_ */
