#
# CMake configuration
#
# Please refer to http://www.cmake.org/cmake/help/documentation.html
# You may also refer to http://www.cmake.org/cmake/help/syntax.html for a quick
# introduction to CMake's syntax.

cmake_minimum_required (VERSION 2.8)

# The name of our project is "NODE". CMakeLists files in this project can
# refer to the root source directory of the project as ${NODE_SOURCE_DIR}
# and to the root binary directory of the project as ${NODE_BINARY_DIR}.
project (BLUE)

# define some more paths to projects we depend on
set (MBED_SRC_PATH     ${BLUE_SOURCE_DIR}/../mbed/libraries/mbed)
set (BLE_API_SRC_PATH  ${BLUE_SOURCE_DIR}/../BLE_API)
set (NRF51822_SRC_PATH ${BLUE_SOURCE_DIR}/../nRF51822)

# It's best to hide all the details of setting up the variable SRCS in a CMake
# macro. The macro can then be called in all the project CMake list files to add
# sources.
#
# The macro first computes the path of the source file relative to the project
# root for each argument. If the macro is invoked from inside a project sub
# directory the new value of the variable SRCS needs to be propagated to the
# parent folder by using the PARENT_SCOPE option.
#
# Source: http://stackoverflow.com/questions/7046956/populating-srcs-from-cmakelists-txt-in-subdirectories
macro (add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()


# decide about the actual compilers to be used ...
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(SIZE_COMMAND       arm-none-eabi-size)
set(OBJCOPY_COMMAND    arm-none-eabi-objcopy)
set(MAIN_TARGET        ${PROJECT_NAME}.elf)
enable_language(ASM)

message(STATUS "C compiler  : ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Size command: ${SIZE_COMMAND}")
message(STATUS "Main target : ${MAIN_TARGET}")

############################################################################
# Build type should be clear from here so we
# can continue with selecting include directors, defines
# and other compiler/linker flags ...
############################################################################
# include directories
include_directories(
    ${BLUE_SOURCE_DIR}
    ${MBED_SRC_PATH}/
    ${MBED_SRC_PATH}/api
    ${MBED_SRC_PATH}/common
    ${MBED_SRC_PATH}/hal
    ${MBED_SRC_PATH}/targets
    ${MBED_SRC_PATH}/targets/cmsis
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM
    ${MBED_SRC_PATH}/targets/hal
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/util
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/crc16
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/nordic_sdk/components/libraries/scheduler
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/TARGET_ARCH_BLE
    ${BLE_API_SRC_PATH}
    ${BLE_API_SRC_PATH}/public
    ${BLE_API_SRC_PATH}/common
    ${BLE_API_SRC_PATH}/services
    ${NRF51822_SRC_PATH}
    ${NRF51822_SRC_PATH}/btle
    ${NRF51822_SRC_PATH}/btle/custom
    ${NRF51822_SRC_PATH}/common
    ${NRF51822_SRC_PATH}/nordic-sdk
    ${NRF51822_SRC_PATH}/nordic-sdk/components
    ${NRF51822_SRC_PATH}/nordic-sdk/components/ble
    ${NRF51822_SRC_PATH}/nordic-sdk/components/ble/ble_radio_notification
    ${NRF51822_SRC_PATH}/nordic-sdk/components/ble/ble_services
    ${NRF51822_SRC_PATH}/nordic-sdk/components/ble/ble_services/ble_dfu
    ${NRF51822_SRC_PATH}/nordic-sdk/components/ble/common
    ${NRF51822_SRC_PATH}/nordic-sdk/components/ble/device_manager
    ${NRF51822_SRC_PATH}/nordic-sdk/components/ble/device_manager/config
    ${NRF51822_SRC_PATH}/nordic-sdk/components/drivers_nrf
    ${NRF51822_SRC_PATH}/nordic-sdk/components/drivers_nrf/ble_flash
    ${NRF51822_SRC_PATH}/nordic-sdk/components/drivers_nrf/hal
    ${NRF51822_SRC_PATH}/nordic-sdk/components/drivers_nrf/pstorage
    ${NRF51822_SRC_PATH}/nordic-sdk/components/drivers_nrf/pstorage/config
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries/bootloader_dfu
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries/bootloader_dfu/experimental
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries/bootloader_dfu/hci_transport
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries/crc16
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries/hci
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries/scheduler
    ${NRF51822_SRC_PATH}/nordic-sdk/components/libraries/util
    ${NRF51822_SRC_PATH}/nordic-sdk/components/softdevice
    ${NRF51822_SRC_PATH}/nordic-sdk/components/softdevice/common
    ${NRF51822_SRC_PATH}/nordic-sdk/components/softdevice/common/softdevice_handler
    ${NRF51822_SRC_PATH}/nordic-sdk/components/softdevice/s130
    ${NRF51822_SRC_PATH}/nordic-sdk/components/softdevice/s130/include
    
)

# Generic compiler flags
add_definitions(
    -mcpu=cortex-m0 -mthumb
    -Wall
    -fdata-sections
    -ffunction-sections
    -fmessage-length=0
    -fno-common
    -fno-exceptions
    -funsigned-bitfields
    -O3
    -g3
    -ggdb
    -DTARGET_M0
    -DTARGET_NORDIC
    -DTOOLCHAIN_GCC
    -D__CORTEX_M0
    -DARM_MATH_CM0
    -D__MBED__=1
    -DNRF51
    -DTARGET_ARCH_BLE 
    -DTARGET_MCU_NRF51822 
    -DTOOLCHAIN_GCC_ARM  
    -DMBED_BUILD_TIMESTAMP=1409211084.44 
    -DTARGET_FF_ARDUINO
    -D__STACK_SIZE=1024
    -D__HEAP_SIZE=1024
)

# Language specifc compiler flags.
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=gnu++98 -fno-rtti -fno-threadsafe-statics")
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_ASM_FLAGS
    "${COMMON_COMPILE_FLAGS} -x assembler-with-cpp")

SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS
    -T${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/TARGET_MCU_NORDIC_16K/NRF51822.ld)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS
    "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -mcpu=cortex-m0 -mthumb --specs=nano.specs -u _printf_float -u _scanf_float -lsupc++ -lm -lgcc -Wl,--start-group -lc -lc -lnosys -Wl,--end-group -Wl,--wrap=main -Wl,--gc-sections")


# A macro to collect local sources into ${SRCS}.
# This variable gets propagated to the parent scope and is ultimately used in
# the top-level CMakeLists.txt to define the dependencies for the build target.
#
# Please note that files within this list are relative to the current folder.
# Please also note that this macro must be used at all CMakeLists.txt files at
# intermediate levels even if the list is empty--this is due to the Cmake magic
# involved in propagating variables to only the parent scope.
add_sources(
    main.cpp
)

# Use file globbing to collect all sources from external repositories. File-
# globbing is discouraged by CMake, except when collecting sources from an
# external source which remains mostly frozen. The risk with globbing is that
# CMake doesn't automatically update the makefiles if new sources are added to
# the globbed location.
#

file(GLOB MBED_SRC_SOURCES
        ${MBED_SRC_PATH}/common/*.c
        ${MBED_SRC_PATH}/common/*.cpp
        ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/*.c
        ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/*.c
        ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/app_common/*.c
    )
add_sources(${MBED_SRC_SOURCES})
add_sources(${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/startup_NRF51822.s)

file(GLOB_RECURSE BLE_API_SOURCES
        ${BLE_API_SRC_PATH}/*.cpp
    )
add_sources(${BLE_API_SOURCES})

file(GLOB_RECURSE NRF51822_SOURCES
        ${NRF51822_SRC_PATH}/*.cpp
        ${NRF51822_SRC_PATH}/*.c
    )
add_sources(${NRF51822_SOURCES})



############################################################################
# By now, we've traversed all subdirectories and have collected everything that
# needs to be built. We can define the build targets.
############################################################################
# add MbedTest as a build target depending on all the sources
add_executable(${MAIN_TARGET} ${SRCS})

# Add a post-build dependency like printing size of the
# resulting binary and copying to the target.
add_custom_command(
    TARGET ${MAIN_TARGET}
    COMMAND ${SIZE_COMMAND} ${MAIN_TARGET}
    #COMMAND ${TOOLCHAIN_SYSROOT}/bin/fromelf --bin -o ${PROJECT_NAME}.bin ${MAIN_TARGET} # convert .elf to .bin
    COMMAND ${OBJCOPY_COMMAND} ${MAIN_TARGET}  -O ihex ${PROJECT_NAME}.hex  # convert .elf to .hex
    COMMAND srec_cat ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s130_nrf51822_1_0_0/s130_nrf51_1.0.0_softdevice.hex -intel ${PROJECT_NAME}.hex -intel -o combined.hex -intel --line-length=44
    # follow this by copying the resulting combined.hex onto the target (possibly over USB)
)
