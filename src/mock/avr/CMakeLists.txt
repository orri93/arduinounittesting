add_library(libavr STATIC eeprom.cpp)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(${arduino_avr_include})
