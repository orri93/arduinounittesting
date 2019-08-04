set(PWFusion_MAX31865_source 
  PWFusion_MAX31865/PlayingWithFusion_MAX31865.cpp
  PWFusion_MAX31865/PlayingWithFusion_MAX31865.h
  PWFusion_MAX31865/PlayingWithFusion_MAX31865_STRUCT.h)
set(MCP3208_source
  MCP3208/MCP3208.cpp
  MCP3208/MCP3208.h)

add_library(libarduinoextern STATIC
# ${MCP3208_source}
  ${PWFusion_MAX31865_source})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(
  ${arduino_mock_include}
  ${CMAKE_SOURCE_DIR}/extern/libraries/MCP3208)
