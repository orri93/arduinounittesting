set(ArduinoModbusSlave_include ArduinoModbusSlave/src)
set(ArduinoModbusSlave_source 
  "${ArduinoModbusSlave_include}/ModbusSlave.cpp"
  "${ArduinoModbusSlave_include}/ModbusSlave.h")

set(PWFusion_MAX31865_include PWFusion_MAX31865)
set(PWFusion_MAX31865_source 
  "${PWFusion_MAX31865_include}/PlayingWithFusion_MAX31865.cpp"
  "${PWFusion_MAX31865_include}/PlayingWithFusion_MAX31865.h"
  "${PWFusion_MAX31865_include}/PlayingWithFusion_MAX31865_STRUCT.h")

set(MCP3208_include MCP3208)
set(MCP3208_source
  "${MCP3208_include}/MCP3208.cpp"
  "${MCP3208_include}/MCP3208.h")

set(Arduino_PID_Library_include Arduino-PID-Library)
set(Arduino_PID_Library_source
  "${Arduino_PID_Library_include}/PID_v1.cpp"
  "${Arduino_PID_Library_include}/PID_v1.h")

add_library(libarduinoextern STATIC
  ${ArduinoModbusSlave_source}
  ${PWFusion_MAX31865_source}
  ${MCP3208_source}
  ${Arduino_PID_Library_source})

add_compile_definitions(ARDUINO=100)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(
  ${arduino_mock_include}
  ${ArduinoModbusSlave_include}
  ${PWFusion_MAX31865_include}
  ${MCP3208_include}
  ${Arduino_PID_Library_include})

#message(STATUS "")
#message(STATUS "BUILD EXTERN ARDUINO LIBRARY SUMMARY")
#message(STATUS "  ARDUINO              : ${ARDUINO}")
#message(STATUS "")
