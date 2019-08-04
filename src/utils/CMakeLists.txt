add_library(libgosutils STATIC
  expect.cpp
  order.cpp
  wordq.cpp)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(
  ${arduino_mock_include}
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinotemplates/src)
