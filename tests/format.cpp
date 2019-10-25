#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <arduinoformat.h>

#define FORMAT_PRECISION  1
#define DISPLAY_LENGTH   12

#define TEXT_UNIT      " C"
#define SENSOR_IDS     "PK"

TEST(ArduinoFormatTest, SetupAndGet) {
  char* pointer = nullptr;
  fds::format::setup();
  pointer = fds::format::get(0);
  ASSERT_NE(pointer, nullptr);
  pointer = fds::format::get(1);
  ASSERT_NE(pointer, nullptr);
  fds::format::free();
}

TEST(ArduinoFormatTest, Number) {
  char* pointer = nullptr;
  fds::format::setup();
  fds::format::number(93.418);
  pointer = fds::format::get(0);
  ASSERT_STREQ(" 93.42", pointer);
  pointer = fds::format::get(1);
  pointer[0] = 'A';
  pointer[1] = ' ';
  fds::format::number(666.11, 1, FORMAT_DOUBLE_WIDTH, 1, 2);
  ASSERT_STREQ("A  666.1", pointer);
  fds::format::free();
}

TEST(ArduinoFormatTest, NumberOnLine) {
  char* pointer = nullptr;
  fds::format::setup();
  fds::format::set('\0', "C", 2, 9, 6, 1);
  fds::format::number(32.89);
  pointer = fds::format::get(0);
  ASSERT_STREQ("  32.9 C", pointer);
  fds::format::free();
}

TEST(ArduinoFormatTest, Main) {
  char* pointer = nullptr;
  fds::format::setup();
  fds::format::set(
    ':',
    TEXT_UNIT,
    sizeof(TEXT_UNIT),
    DISPLAY_LENGTH,
    DISPLAY_LENGTH - sizeof(TEXT_UNIT) - 2,
    FORMAT_PRECISION);
  fds::format::ids(SENSOR_IDS, sizeof(SENSOR_IDS));
  fds::format::number(93.418);
  pointer = fds::format::get();
  ASSERT_STREQ("P:   93.4 C", pointer);
  fds::format::number(666.11, 1, FORMAT_DOUBLE_WIDTH, FORMAT_DOUBLE_PRECISION, 2);
  pointer = fds::format::get(1);
  pointer[0] = 'A';
  pointer[1] = ' ';
  ASSERT_STREQ("A   666.1 C", pointer);
  fds::format::free();
}

