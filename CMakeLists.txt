cmake_minimum_required(VERSION 3.1...3.15)

project(ArduinoUnitTesting VERSION 1.0
  DESCRIPTION "FDS Arduino libraries Google unit tests"
  LANGUAGES CXX)

option(FDS_PACKAGE_TESTS "Build tests" ON)

#add_compile_definitions(ARDUINO=200)
#add_compile_definitions(FDS_ARDUINO_UNIT_TESTING=200)
#add_compile_definitions(MEDIAN_SORT_INSERT)

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
add_subdirectory(sublibraries/arduinomedian)

if(FDS_PACKAGE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

mark_as_advanced(
BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
gmock_build_tests gtest_build_samples gtest_build_tests
gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

add_executable(ArduinoUnitTesting tests/median.cpp)

#set_target_properties(gtest PROPERTIES FOLDER extern)
#set_target_properties(gtest_main PROPERTIES FOLDER extern)
#set_target_properties(gmock PROPERTIES FOLDER extern)
#set_target_properties(gmock_main PROPERTIES FOLDER extern)

include_directories(
#  ${Boost_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include
  ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include
  ${CMAKE_SOURCE_DIR}/extern/arduinomock
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinomedian/src)

target_link_libraries(ArduinoUnitTesting
  gtest
  gmock
  gtest_main
  libarduinomock
  libarduinomedian)

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
