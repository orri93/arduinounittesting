add_library(libgosutils STATIC
# expect.cpp
  order.cpp
  wordq.cpp)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(
  ${arduino_mock_include}
  "${arduino_unit_testing_sublibraries_dir}/arduinotemplates/src")
