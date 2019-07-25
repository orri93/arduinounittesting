cmake_minimum_required(VERSION 3.1...3.15)

project(ExecuteUnitTests VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include)

add_executable(ExecuteUnitTests execute.cpp)

target_link_libraries(ExecuteUnitTests PRIVATE arduino-mock)
target_link_libraries(ExecuteUnitTests PRIVATE ArduinoMedianLib)
target_link_libraries(ExecuteUnitTests PRIVATE ArduinoMedianTestLib)
