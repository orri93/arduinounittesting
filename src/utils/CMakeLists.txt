add_library(libgosutils STATIC
  expect.cpp
  order.cpp
  wordq.cpp)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(
  ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include 
  ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include
  ${arduino_mock_include}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/sublibraries/arduinotemplates/src)
