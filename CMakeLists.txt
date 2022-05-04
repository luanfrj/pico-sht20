cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_sht20)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (TARGET tinyusb_device)
    add_executable(pico_sht20 pico_sht20.c)

    # pull in common dependencies and additional i2c hardware support
    target_link_libraries(pico_sht20 pico_stdlib hardware_i2c)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(pico_sht20 1)
    pico_enable_stdio_uart(pico_sht20 0)

    # create map/bin/hex file etc.
    pico_add_extra_outputs(pico_sht20)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building pico_sht20 because TinyUSB submodule is not initialized in the SDK")
endif()
