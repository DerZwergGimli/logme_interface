cmake_minimum_required(VERSION 3.16)

# This example uses an extra component for common functions such as Wi-Fi and Ethernet connection.
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)


#set(CMAKE_FIND_ROOT_PATH /home/yannick/.espressif/tools/xtensa-esp32-elf/esp-2022r1-11.2.0/xtensa-esp32-elf/)
#set(CMAKE_C_COMPILER /home/yannick/.espressif/tools/xtensa-esp32-elf/esp-2022r1-11.2.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc)
#set(CMAKE_CXX_COMPILER /home/yannick/.espressif/tools/xtensa-esp32-elf/esp-2022r1-11.2.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-g++)
#set(CMAKE_ASM_COMPILER /home/yannick/.espressif/tools/xtensa-esp32-elf/esp-2022r1-11.2.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)


project(logme_interface)
