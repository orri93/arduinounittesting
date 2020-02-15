#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlstring.h>

#define GATL_TEST_TEXT_LENGTH 64
#define GATL_TEST_TEXT "Þetta er texti til prófunar á String"
/*                      123456789012345678901234567890123456 */
/*                              10        20        30       */

namespace ga = ::gos::atl;

typedef ga::buffer::Holder<uint16_t, uint8_t> Buffer;

static unsigned long sum(const Buffer& buffer);
static unsigned long sum(const char* text);

TEST(GatlStringTest, Copy) {
  unsigned long e, s;

  Buffer buffer(GATL_TEST_TEXT_LENGTH);

  ga::buffer::clear(buffer);
  s = sum(buffer);
  EXPECT_EQ(0, s);

  ga::string::copy(buffer, GATL_TEST_TEXT);
  e = sum(GATL_TEST_TEXT);
  s = sum(buffer);
  EXPECT_EQ(e, s);
}

TEST(GatlStringTest, Compare) {
  int cr;

  Buffer buffer(GATL_TEST_TEXT_LENGTH);

  ga::buffer::clear(buffer);
  cr = ga::string::compare(buffer, GATL_TEST_TEXT);
  EXPECT_NE(0, cr);

  ::memcpy(buffer.Buffer, GATL_TEST_TEXT, sizeof(GATL_TEST_TEXT));
  cr = ga::string::compare(buffer, GATL_TEST_TEXT);
  EXPECT_EQ(0, cr);
}

unsigned long sum(const Buffer& buffer) {
  unsigned long s = 0;
  for (uint16_t i = 0; i < buffer.Size; ++i) {
    uint8_t c = buffer.Buffer[i];
    s += c;
  }
  return s;
}

unsigned long sum(const char* text) {
  unsigned long s = 0;
  size_t size = ::strnlen(text, GATL_TEST_TEXT_LENGTH);
  for (size_t i = 0; i < size; ++i) {
    uint8_t c = static_cast<uint8_t>(text[i]);
    s += c;
  }
  return s;
}
