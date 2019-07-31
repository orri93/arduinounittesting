cmake_minimum_required(VERSION 3.1...3.15)

project(ArduinoUnitTesting VERSION 1.0
  DESCRIPTION "FDS Arduino libraries Google unit tests"
  LANGUAGES CXX)

set(arduino_unit_testing_src
  tests/format.cpp
  tests/median.cpp
  tests/sensor.cpp
  tests/max31865
  tests/max6675
  tests/sort.cpp
)

set(arduino_mock_include
  ${CMAKE_SOURCE_DIR}/extern/arduinomock)
#  ${CMAKE_SOURCE_DIR}/extern/arduinomock/include/arduino-mock)
set(arduino_mock_for_lib_include 
  arduinomock)
#  arduinomock/include/arduino-mock)

option(FDS_PACKAGE_TESTS "Build tests" ON)

#add_compile_definitions(ARDUINO=200)
#add_compile_definitions(FDS_ARDUINO_UNIT_TESTING=200)
add_compile_definitions(GOS_ARDUINO_UNIT_TESTING)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  RESULT_VARIABLE GIT_SUBMOD_RESULT)
  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
  endif()
endif()

set(gtest_force_shared_crt ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

#set(Boost_USE_STATIC_LIBS       OFF)  # only find static libs
#set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
#set(Boost_USE_RELEASE_LIBS      OFF)  # only find release libs
#set(Boost_USE_MULTITHREADED     OFF)
#set(Boost_USE_STATIC_RUNTIME    OFF)

#find_package(Boost 1.70.0 COMPONENTS boost)

add_subdirectory(extern)
add_subdirectory(src/mock)
add_subdirectory(src/avr-libc)
add_subdirectory(sublibraries/arduinomax6675)
add_subdirectory(sublibraries/arduinomax31865)
add_subdirectory(sublibraries/arduinosensor)
add_subdirectory(sublibraries/arduinoformat)
add_subdirectory(sublibraries/arduinomedian)
add_subdirectory(sublibraries/arduinosort)

if(FDS_PACKAGE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

mark_as_advanced(
BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
gmock_build_tests gtest_build_samples gtest_build_tests
gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

add_executable(ArduinoUnitTesting ${arduino_unit_testing_src})

#set_target_properties(gtest PROPERTIES FOLDER extern)
#set_target_properties(gtest_main PROPERTIES FOLDER extern)
#set_target_properties(gmock PROPERTIES FOLDER extern)
#set_target_properties(gmock_main PROPERTIES FOLDER extern)

include_directories(
#  ${Boost_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include
  ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include
  ${arduino_mock_include}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/avr-libc/libc/stdlib
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinomax6675/src
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinomax31865/src
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinosensor/src
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinoformat/src
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinomedian/src
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinosort/src)

target_link_libraries(ArduinoUnitTesting
  gtest
  gmock
  gtest_main
  libmock
  libavrlibc
  libarduinomock
  libarduinomax6675
  libarduinomax31865
  libarduinosensor
  libarduinoformat
  libarduinomedian
  libarduinosort)

message(STATUS "")
message(STATUS "BUILD SUMMARY")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  Compiler ID          : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build tests          : ${FDS_PACKAGE_TESTS}")
message(STATUS "  Force Shared CRT     : ${gtest_force_shared_crt}")
#message(STATUS "  Boost version        : ${Boost_VERSION}")
#message(STATUS "  Boost include dir    : ${Boost_INCLUDE_DIRS}")
message(STATUS "")
