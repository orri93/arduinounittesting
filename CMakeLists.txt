cmake_minimum_required(VERSION 3.1...3.15)

project(ArduinoUnitTesting VERSION 1.0
  DESCRIPTION "FDS Arduino libraries Google unit tests"
  LANGUAGES CXX)

option(GOS_ARDUINO_UT_BUILD_TESTING "Build Arduino unit tests folder" ON)
option(GOS_ARDUINO_UT_BUILD_MCP_3208 "Build Arduino MCP 3208" ON)
option(GOS_ARDUINO_UT_BUILD_MAX_6675 "Build Arduino MAX 6675" ON)
option(GOS_ARDUINO_UT_BUILD_MAX_31865 "Build Arduino MAX 13865" ON)
option(GOS_ARDUINO_UT_BUILD_SENSOR "Build Arduino sensor" ON)
option(GOS_ARDUINO_UT_BUILD_SIN_LED "Build Arduino sin LED" ON)
option(GOS_ARDUINO_UT_BUILD_FORMAT "Build Arduino format" ON)
option(GOS_ARDUINO_UT_BUILD_MEDIAN "Build Arduino median" ON)
option(GOS_ARDUINO_UT_BUILD_SORT "Build Arduino sort" ON)
option(GOS_ARDUINO_UT_BUILD_TICK "Build Arduino tick" ON)

set(arduino_unit_testing_root_include "${CMAKE_CURRENT_SOURCE_DIR}/include")

list(APPEND arduino_unit_testing_src
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/led.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/crc.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/pid.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/sort.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/type.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/avrage.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/string.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/buffer.cpp"
#  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/eeprom.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/format.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/median.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/modbus.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/binding.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/utility.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/display.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/gatl/sensor.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixedpoints.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/sensor.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/max6675")

list(APPEND arduino_mock_for_lib_include 
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
#  "${CMAKE_CURRENT_SOURCE_DIR}/include/avr"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/arduinomock"
# "${CMAKE_CURRENT_SOURCE_DIR}/extern/arduinomock/include/arduino-mock"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest/googlemock/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest/googletest/include")

set(arduino_unit_testing_sublibraries_dir
  ${CMAKE_CURRENT_SOURCE_DIR}/sublibraries)

list(APPEND arduino_mock_include
  "${arduino_mock_for_lib_include}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/mock"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/libraries"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/cnl/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/fixed_point/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/libraries/FixedPointsArduino/src"
  "${arduino_unit_testing_sublibraries_dir}/arduinotemplates/src"
  )

list(APPEND sublibraries_include ${arduino_mock_include})

list(APPEND arduino_testing_include
# "${Boost_INCLUDE_DIRS}"
  "${arduino_mock_include}"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/avr-libc/libc/stdlib"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/libraries/FixedPointsArduino/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/libraries/Arduino-PID-Library"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/libraries/PWFusion_MAX31865"
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/libraries/MCP3208"
  "${arduino_unit_testing_sublibraries_dir}/arduinotemplates/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/sublibraries/arduinomcp3208/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/sublibraries/arduinomax6675/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/sublibraries/arduinomax31865/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/sublibraries/arduinosensor/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/sublibraries/arduinoformat/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/sublibraries/arduinomedian/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/sublibraries/arduinosort/src")

list(APPEND arduino_avr_include
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
  #"${CMAKE_CURRENT_SOURCE_DIR}/include/avr")

list(APPEND arduino_testing_target_link_libraries
  gtest
  gmock
  gmock_main
  libavr
  libmock
  libavrlibc
  libgosutils
  libarduinomock
  libarduinoextern)

set(gtest_force_shared_crt ON CACHE BOOL "Prefer to use Shared CRT for static")

#add_compile_definitions(P_ON_M=0)
#add_compile_definitions(P_ON_E=1)

#add_compile_definitions(FDS_ARDUINO_UNIT_TESTING=200)
add_compile_definitions(GATL_PID_DEBUG)
add_compile_definitions(GOS_ARDUINO_TEMPLATE_LIBRARY_CHECK_SIZE)
add_compile_definitions(GOS_ARDUINO_TEMPLATE_LIBRARY_CLEANUP)
add_compile_definitions(GOS_ARDUINO_TEMPLATE_LIBRARY_MODBUS_TESTING)
add_compile_definitions(GOS_ARDUINO_TEMPLATE_LIBRARY_BINDING_TESTING)
add_compile_definitions(GOS_ARDUINO_UNIT_TESTING)
add_compile_definitions(FIXED_POINTS_USE_NAMESPACE)
add_compile_definitions(FIXED_POINTS_NO_RANDOM)
add_compile_definitions(FIXED_POINTS_NO_CONSTANTS)

#find_package(Git QUIET)
#if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
#  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#  RESULT_VARIABLE GIT_SUBMOD_RESULT)
#  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
#    message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#  endif()
#endif()

#
# Beter to do this from outside
#
#set(gtest_force_shared_crt ON)

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
add_subdirectory(src/mock/avr)
add_subdirectory(src/utils)
add_subdirectory(src/avr-libc)
if (GOS_ARDUINO_UT_BUILD_MCP_3208)
  add_subdirectory(sublibraries/arduinomcp3208)
  list(APPEND arduino_testing_target_link_libraries
    libarduinomcp3208)
endif ()
if (GOS_ARDUINO_UT_BUILD_MAX_6675)
  add_subdirectory(sublibraries/arduinomax6675)
  list(APPEND arduino_testing_target_link_libraries
    libarduinomax6675)
endif ()
if (GOS_ARDUINO_UT_BUILD_MAX_31865)
  add_subdirectory(sublibraries/arduinomax31865)
  list(APPEND arduino_unit_testing_src
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/max31865")
  list(APPEND arduino_testing_target_link_libraries
    libarduinomax31865)
endif ()
if (GOS_ARDUINO_UT_BUILD_SENSOR)
  add_subdirectory(sublibraries/arduinosensor)
  list(APPEND arduino_testing_target_link_libraries
    libarduinosensor)
endif ()
if (GOS_ARDUINO_UT_BUILD_SIN_LED)
  add_subdirectory(sublibraries/arduinosinled)
endif ()
if (GOS_ARDUINO_UT_BUILD_FORMAT)
  add_subdirectory(sublibraries/arduinoformat)
  list(APPEND arduino_unit_testing_src
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/format.cpp")
  list(APPEND arduino_testing_target_link_libraries
    libarduinoformat)
endif ()
if (GOS_ARDUINO_UT_BUILD_MEDIAN)
  add_subdirectory(sublibraries/arduinomedian)
  list(APPEND arduino_unit_testing_src
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/median.cpp")
  list(APPEND arduino_testing_target_link_libraries
    libarduinomedian)
endif ()
if (GOS_ARDUINO_UT_BUILD_SORT)
  add_subdirectory(sublibraries/arduinosort)
  list(APPEND arduino_unit_testing_src
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/sort.cpp")
  list(APPEND arduino_testing_target_link_libraries
    libarduinosort)
endif ()
if (GOS_ARDUINO_UT_BUILD_TICK)
  add_subdirectory(sublibraries/arduinotick)
endif ()
add_subdirectory(extern/libraries)

if (GOS_ARDUINO_UT_BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif ()

message(STATUS "")
message(STATUS "Arduino Unit Testing BUILD SUMMARY")
message(STATUS "  ARDUINO                   : ${ARDUINO}")
message(STATUS "  CMAKE_GENERATOR           : ${CMAKE_GENERATOR}")
message(STATUS "  CMAKE_SOURCE_DIR          : ${CMAKE_SOURCE_DIR}")
message(STATUS "  CMAKE_CURRENT_SOURCE_DIR  : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  CMAKE_GENERATOR           : ${CMAKE_GENERATOR}")
message(STATUS "  Compiler ID               : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type                : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build tests               : ${FDS_PACKAGE_TESTS}")
message(STATUS "  Force Shared CRT          : ${gtest_force_shared_crt}")
#message(STATUS "  Boost version             : ${Boost_VERSION}")
#message(STATUS "  Boost include dir         : ${Boost_INCLUDE_DIRS}")
message(STATUS "")
