cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(system_two_servo)

target_sources(app PRIVATE src/main.c)

if(CONFIG_CUSTOM_SERVO)
    target_sources(app PRIVATE drivers/servo/servo.c)
    target_include_directories(app PRIVATE drivers/servo)
endif()
