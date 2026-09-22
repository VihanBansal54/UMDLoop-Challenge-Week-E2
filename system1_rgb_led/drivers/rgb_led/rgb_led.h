#ifndef RGB_LED_H_
#define RGB_LED_H_

#include <zephyr/device.h>
#include <stdint.h>

/**
 * @brief Light whichever channel (red, green, or blue) is dominant in hex.
 *
 * @param dev Pointer to the rgb-led device instance.
 * @param hex 24-bit color value, e.g. 0xFF0000 for red.
 * @return 0 on success, negative errno on failure.
 */
int rgb_led_set_from_hex(const struct device *dev, uint32_t hex);

#endif /* RGB_LED_H_ */
