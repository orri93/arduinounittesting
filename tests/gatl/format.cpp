#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlformat.h>

#define TEXT_ID "A:"
#define TEXT_UNIT " C"
#define TEXT_ERROR "Failure"

namespace gatl = ::gos::atl;

TEST(GatlFormatTest, Format) {
  const uint8_t size = 11;
  gatl::format::option::Number option;
  gatl::buffer::Holder<uint8_t> buffer(size);
  gatl::buffer::Holder<uint8_t> id(TEXT_ID, sizeof(TEXT_ID));
  gatl::buffer::Holder<uint8_t> unit(TEXT_UNIT, sizeof(TEXT_UNIT));

  size_t length;
  bool checkresult;
  double real = 93.418;
  int integer = 666;
  checkresult = gatl::format::check::real(buffer, option, &id, &unit);
  EXPECT_TRUE(checkresult);
  gatl::format::real<double>(buffer, real, option, &id, &unit);
  EXPECT_STREQ("A:  93.4 C", buffer.Buffer);
  length = strlen(buffer.Buffer);
  EXPECT_EQ(size - 1, length);

  checkresult = gatl::format::check::real(buffer, option, &id);
  EXPECT_TRUE(checkresult);
  gatl::format::real<double>(buffer, real, option, &id);
  EXPECT_STREQ("A:    93.4", buffer.Buffer);
  length = strlen(buffer.Buffer);
  EXPECT_EQ(size - 1, length);

  checkresult = gatl::format::check::real(buffer, option);
  EXPECT_TRUE(checkresult);
  gatl::format::real<double>(buffer, real, option);
  EXPECT_STREQ("      93.4", buffer.Buffer);
  length = strlen(buffer.Buffer);
  EXPECT_EQ(size - 1, length);

  option.Width = 6;
  option.Precision = 2;
  checkresult = gatl::format::check::real(buffer, option);
  EXPECT_TRUE(checkresult);
  gatl::format::real<double>(buffer, real, option);
  EXPECT_STREQ(" 93.42", buffer.Buffer);
  length = strlen(buffer.Buffer);
  EXPECT_EQ(6, length);

  option.Width = -7;
  checkresult = gatl::format::check::real(buffer, option);
  EXPECT_TRUE(checkresult);
  gatl::format::real<double>(buffer, real, option);
  EXPECT_STREQ("93.42  ", buffer.Buffer);
  length = strlen(buffer.Buffer);
  EXPECT_EQ(7, length);

  option.Width = GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_FILL_NEGATIVE;
  checkresult = gatl::format::check::real(buffer, option, &id, &unit);
  EXPECT_TRUE(checkresult);
  gatl::format::real<double>(buffer, real, option, &id, &unit);
  EXPECT_STREQ("A:93.42  C", buffer.Buffer);
  length = strlen(buffer.Buffer);
  EXPECT_EQ(size - 1, length);

  checkresult = gatl::format::check::integer(
    buffer,
    integer,
    &id,
    &unit,
    GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_UNDEFINED);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(
    buffer,
    integer,
    &id,
    &unit,
    GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_UNDEFINED);
  EXPECT_STREQ("A:666 C", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer, &id, &unit, 6);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer, &id, &unit, 6);
  EXPECT_STREQ("A:   666 C", buffer.Buffer);

  checkresult = gatl::format::check::integer<int, uint8_t>(buffer, integer, &id, nullptr, 6);
  EXPECT_TRUE(checkresult);
  gatl::format::integer<int, uint8_t>(buffer, integer, &id, nullptr, 6);
  EXPECT_STREQ("A:   666", buffer.Buffer);

  checkresult = gatl::format::check::integer<int, uint8_t>(buffer, integer, nullptr, nullptr, 6);
  EXPECT_TRUE(checkresult);
  gatl::format::integer<int, uint8_t>(buffer, integer, nullptr, nullptr, 6);
  EXPECT_STREQ("   666", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer, &id, &unit);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer, &id, &unit);
  EXPECT_STREQ("A:   666 C", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer);
  EXPECT_STREQ("       666", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer, &id, &unit, GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_FILL_NEGATIVE);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer, &id, &unit, GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_FILL_NEGATIVE);
  EXPECT_STREQ("A:666    C", buffer.Buffer);

  integer = -93;
  checkresult = gatl::format::check::integer(
    buffer,
    integer,
    &id,
    &unit,
    GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_UNDEFINED);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(
    buffer,
    integer,
    &id,
    &unit,
    GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_UNDEFINED);
  EXPECT_STREQ("A:-93 C", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer, &id, &unit, 6);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer, &id, &unit, 6);
  EXPECT_STREQ("A:   -93 C", buffer.Buffer);

  checkresult = gatl::format::check::integer<int, uint8_t>(buffer, integer, &id, nullptr, 6);
  EXPECT_TRUE(checkresult);
  gatl::format::integer<int, uint8_t>(buffer, integer, &id, nullptr, 6);
  EXPECT_STREQ("A:   -93", buffer.Buffer);

  checkresult = gatl::format::check::integer<int, uint8_t>(buffer, integer, nullptr, nullptr, 6);
  EXPECT_TRUE(checkresult);
  gatl::format::integer<int, uint8_t>(buffer, integer, nullptr, nullptr, 6);
  EXPECT_STREQ("   -93", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer, &id, &unit);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer, &id, &unit);
  EXPECT_STREQ("A:   -93 C", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer);
  EXPECT_STREQ("       -93", buffer.Buffer);

  checkresult = gatl::format::check::integer(buffer, integer, &id, &unit, GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_FILL_NEGATIVE);
  EXPECT_TRUE(checkresult);
  gatl::format::integer(buffer, integer, &id, &unit, GOS_ARDUINO_TEMPLATE_LIBRARY_WIDTH_FILL_NEGATIVE);
  EXPECT_STREQ("A:-93    C", buffer.Buffer);

  gatl::format::message(buffer, TEXT_ERROR, sizeof(TEXT_ERROR), &id);
  EXPECT_STREQ("A:Failure", buffer.Buffer);

  gatl::format::message(buffer, TEXT_ERROR, sizeof(TEXT_ERROR));
  EXPECT_STREQ("Failure", buffer.Buffer);

  buffer.cleanup();
}
