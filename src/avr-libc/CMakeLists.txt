add_library(libavrlibc STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/libc/stdlib/dtoa_prf.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/libc/stdlib/dtostrf.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/libc/stdlib/ftoa_engine.cc")

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

include_directories(
  ${arduino_avr_include}
  ${arduino_unit_testing_root_include}/avr)
