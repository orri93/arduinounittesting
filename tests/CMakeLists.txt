#include(GoogleTest)

#find_package(Boost 1.70.0 COMPONENTS boost)

set(executegtests_target executegtests)

add_executable(${executegtests_target} ${arduino_unit_testing_src})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

if (MSVC)
  target_compile_options(${executegtests_target} PRIVATE /wd4244)
endif()

target_compile_definitions(${executegtests_target} PUBLIC ARDUINO_ARCH_AVR)

target_include_directories(${executegtests_target} PRIVATE
  ${arduino_testing_include})

target_link_libraries(${executegtests_target}
  ${arduino_testing_target_link_libraries}
  libarduinomodbusslave)

#gtest_add_tests(TARGET tests TEST_PREFIX old:)
#gtest_discover_tests(tests TEST_PREFIX new:)
add_test(NAME tests COMMAND executegtests)
