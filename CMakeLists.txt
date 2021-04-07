cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
include(pico-sdk/pico_sdk_init.cmake)



project(my_project VERSION 1.0.0 LANGUAGES CXX C ASM)

add_executable(hello_world
	main.cpp
)
set_property(TARGET hello_world PROPERTY CXX_STANDARD 14)

# --------------------------------------------
# Pull in our pico_stdlib which pulls in commonly used features
pico_sdk_init()

target_link_libraries(hello_world PRIVATE pico_stdlib pico_multicore hardware_adc hardware_gpio )
target_link_libraries(hello_world PUBLIC pico_bootsel_via_double_reset) # Easier booting ? 
pico_enable_stdio_usb(hello_world 1)
pico_enable_stdio_uart(hello_world 1)

pico_add_extra_outputs(hello_world)


# --------------------------------------------
# Boost Library setup
include(libboost.cmake)
target_include_directories(hello_world PRIVATE ${Boost_INCLUDE_DIRS})

# --------------------------------------------
# CMSIS Dependancy for Cortex M0+
target_include_directories(hello_world INTERFACE CMSIS_5/CMSIS/Core/Include/core_cm0plus.h)

# --------------------------------------------
# Type_expr
add_subdirectory(type_expr)
set(TYPE_EXPR_TEST OFF)
target_link_libraries(hello_world PRIVATE type_expr)



