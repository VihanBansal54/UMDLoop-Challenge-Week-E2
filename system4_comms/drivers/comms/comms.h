#ifndef COMMS_H_
#define COMMS_H_

#include <zephyr/device.h>
#include <stdint.h>

/**
 * @brief Try to receive one byte (0-255) over the wrapped UART.
 *
 * @return 0 and fills *out_byte if a byte was available, -1 otherwise.
 */
int comms_receive_byte(const struct device *dev, uint8_t *out_byte);

#endif /* COMMS_H_ */
