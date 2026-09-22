cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(system_three_tone)

target_sources(app PRIVATE src/main.c)

if(CONFIG_CUSTOM_TONE)
    target_sources(app PRIVATE drivers/tone/tone.c)
    target_include_directories(app PRIVATE drivers/tone)
endif()
