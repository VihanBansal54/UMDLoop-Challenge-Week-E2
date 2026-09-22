cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(system_one_rgb_led)

target_sources(app PRIVATE src/main.c)

if(CONFIG_CUSTOM_RGB_LED)
    target_sources(app PRIVATE drivers/rgb_led/rgb_led.c)
    target_include_directories(app PRIVATE drivers/rgb_led)
endif()
