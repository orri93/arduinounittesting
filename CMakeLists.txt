cmake_minimum_required(VERSION 3.1...3.15)

project(ArduinoUnitTesting VERSION 1.0
  DESCRIPTION "FDS Arduino libraries Google unit tests"
  LANGUAGES CXX)

add_subdirectory(extern)
add_subdirectory(sublibraries)
add_subdirectory(tests)
add_subdirectory(src)

target_include_directories(arduino-mock PUBLIC include)
