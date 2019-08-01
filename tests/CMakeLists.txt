#include(GoogleTest)

#find_package(Boost 1.70.0 COMPONENTS boost)

add_executable(executegtests ${arduino_unit_testing_src})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(${arduino_testing_include})

target_link_libraries(executegtests
  ${arduino_testing_target_link_libraries})

#gtest_add_tests(TARGET tests TEST_PREFIX old:)
#gtest_discover_tests(tests TEST_PREFIX new:)
add_test(NAME tests COMMAND executegtests)
