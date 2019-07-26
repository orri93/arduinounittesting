#include(GoogleTest)

set(test_src
  median.cpp
  sort.cpp
)

#find_package(Boost 1.70.0 COMPONENTS boost)

add_executable(executegtests ${test_src})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(
#  ${Boost_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/extern/boost
  ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include
  ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include
  ${CMAKE_SOURCE_DIR}/extern/arduinomock
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinomedian/src
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinosort/src)

target_link_libraries(executegtests
  gtest
  gmock
  gtest_main
  gmock_main
  libarduinomock
  libarduinomedian
  libarduinosort)

#gtest_add_tests(TARGET tests TEST_PREFIX old:)
#gtest_discover_tests(tests TEST_PREFIX new:)
add_test(NAME tests COMMAND executegtests)
