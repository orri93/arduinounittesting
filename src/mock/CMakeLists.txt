add_library(libmock STATIC
  SPI.cpp)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(${arduino_mock_include})
