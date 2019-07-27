add_library(libavrlibc STATIC
  libc/stdlib/ftoa_engine.cc
  libc/stdlib/dtoa_prf.cc
  libc/stdlib/dtostrf.cc
  libc/stdlib/dtostrf.h
  libc/stdlib/ftoa_engine.h
  libc/stdlib/dtoa_conv.h)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

#include_directories(
#  ${CMAKE_SOURCE_DIR}/extern/googletest/googletest/include 
#  ${CMAKE_SOURCE_DIR}/extern/googletest/googlemock/include
#  ${CMAKE_SOURCE_DIR}/extern/arduinomock)
