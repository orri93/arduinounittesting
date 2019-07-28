add_library(libarduinomock STATIC arduinomock/src/ArduinoMockAll.cc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

add_subdirectory(googletest)

include_directories(
  ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include googletest/googlemock/include
  ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include googletest/googletest/include
  ${arduino_mock_include} ${arduino_mock_for_lib_include})
