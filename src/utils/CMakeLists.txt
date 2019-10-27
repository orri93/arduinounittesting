add_library(libgosutils STATIC
# expect.cpp
  memory.cpp
  order.cpp
  wordq.cpp)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

if (MSVC)
  target_compile_options(libgosutils PRIVATE /wd4244)
endif()


include_directories(
  ${arduino_mock_include}
  "${arduino_unit_testing_sublibraries_dir}/arduinotemplates/src")
