cmake_minimum_required(VERSION 3.1...3.15)

project(ArduinoMedianTests VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(${CMAKE_SOURCE_DIR}/sublibraries/arduinomedian/src)
include_directories(${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/arduinomock)

add_executable(ArduinoMedianTests median.cpp execute.cpp)

target_link_libraries(ArduinoMedianTests PUBLIC gmock)
target_link_libraries(ArduinoMedianTests PUBLIC gtest)
target_link_libraries(ArduinoMedianTests PUBLIC arduino-mock)
target_link_libraries(ArduinoMedianTests PUBLIC ArduinoMedianLib)
