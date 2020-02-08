add_library(libmock STATIC
  SPI.cpp
  U8g2lib.cpp
# WAString.cpp
# ModbusSlave.cpp
  )

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(${arduino_mock_include})
