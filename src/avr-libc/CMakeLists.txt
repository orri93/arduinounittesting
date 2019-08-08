add_library(libavrlibc STATIC
  libc/stdlib/ftoa_engine.cc
  libc/stdlib/dtoa_prf.cc
  libc/stdlib/dtostrf.cc)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(${arduino_avr_include})
