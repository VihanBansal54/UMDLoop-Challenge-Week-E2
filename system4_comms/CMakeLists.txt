cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(system_four_comms)

target_sources(app PRIVATE src/main.c)

if(CONFIG_CUSTOM_COMMS)
    target_sources(app PRIVATE drivers/comms/comms.c)
    target_include_directories(app PRIVATE drivers/comms)
endif()
