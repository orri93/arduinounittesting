#include <gtest/gtest.h>

#include <Arduino.h>

#include <FixedPointsCommon.h>

#include <gatltype.h>
#include <gatlutility.h>

#include <gos/utils/expect.h>

namespace fp = ::FixedPoints;
namespace gatl = ::gos::atl;

TEST(GatlUtilTest, RangeIsMemberOf) {
  uint16_t value, start, length;
  bool result;

  value = 5; start = 10; length = 5;
  result = gatl::utility::range::ismemberof(value, start, length);
  EXPECT_FALSE(result);

  value = 9; start = 10; length = 5;
  result = gatl::utility::range::ismemberof(value, start, length);
  EXPECT_FALSE(result);

  value = 10; start = 10; length = 5;
  result = gatl::utility::range::ismemberof(value, start, length);
  EXPECT_TRUE(result);

  value = 12; start = 10; length = 5;
  result = gatl::utility::range::ismemberof(value, start, length);
  EXPECT_TRUE(result);
  
  value = 14; start = 10; length = 5;
  result = gatl::utility::range::ismemberof(value, start, length);
  EXPECT_TRUE(result);

  value = 15; start = 10; length = 5;
  result = gatl::utility::range::ismemberof(value, start, length);
  EXPECT_FALSE(result);

  value = 20; start = 10; length = 5;
  result = gatl::utility::range::ismemberof(value, start, length);
  EXPECT_FALSE(result);
}

TEST(GatlUtilTest, RangeRestrict) {
  typedef fp::SQ15x16 T;

  const T minimum = -12.34;
  const T maximum = 23.45;
  const T inside = 1.23;
  const T below = -12.345;
  const T above = 23.456;

  T restricted;

  restricted = gatl::utility::range::restrict(inside, minimum, maximum);
  GOS_ARDUINO_TESTING_EQ_FP(inside, restricted);

  restricted = gatl::utility::range::restrict(minimum, minimum, maximum);
  GOS_ARDUINO_TESTING_EQ_FP(minimum, restricted);

  restricted = gatl::utility::range::restrict(maximum, minimum, maximum);
  GOS_ARDUINO_TESTING_EQ_FP(maximum, restricted);

  restricted = gatl::utility::range::restrict(below, minimum, maximum);
  GOS_ARDUINO_TESTING_EQ_FP(minimum, restricted);

  restricted = gatl::utility::range::restrict(above, minimum, maximum);
  GOS_ARDUINO_TESTING_EQ_FP(maximum, restricted);

  gatl::type::range<T> range = gatl::type::make_range(minimum, maximum);

  restricted = gatl::utility::range::restrict(inside, range);
  GOS_ARDUINO_TESTING_EQ_FP(inside, restricted);

  restricted = gatl::utility::range::restrict(minimum, range);
  GOS_ARDUINO_TESTING_EQ_FP(minimum, restricted);

  restricted = gatl::utility::range::restrict(maximum, range);
  GOS_ARDUINO_TESTING_EQ_FP(maximum, restricted);

  restricted = gatl::utility::range::restrict(below, range);
  GOS_ARDUINO_TESTING_EQ_FP(minimum, restricted);

  restricted = gatl::utility::range::restrict(above, range);
  GOS_ARDUINO_TESTING_EQ_FP(maximum, restricted);
}

TEST(GatlUtilTest, NumberPart) {
  typedef fp::SQ15x16 T;

  uint8_t l8b, h8b;
  uint16_t v16b, l16b, h16b;
  uint32_t v32b, l32b, h32b;
  uint64_t v64b;
  T fpv;

  const uint8_t ch8b = 0x12;
  const uint8_t cl8b = 0x34;
  const uint16_t c16b = cl8b +
    (static_cast<uint16_t>(ch8b) << (8 * sizeof(uint8_t)));
  const uint16_t ch16b = 0x8765;
  const uint16_t cl16b = 0x4321;
  const uint32_t c32b = cl16b +
    (static_cast<uint32_t>(ch16b) << (8 * sizeof(uint16_t)));
  const uint32_t c64bl = c16b +
    (static_cast<uint64_t>(c16b) << 16);
  const uint32_t c64bh = c32b;
  const uint64_t c64b = c64bl +
    (static_cast<uint64_t>(c64bh) << (8 * sizeof(uint32_t)));

  EXPECT_EQ(0x1234, c16b);
  EXPECT_EQ(0x87654321, c32b);
  EXPECT_EQ(0x8765432112341234, c64b);

  const T cfpv = 1.2345;

  v16b = c16b;
  v32b = c32b;
  v64b = c64b;
  fpv = cfpv;


  l8b = gatl::utility::number::part::first<>(v16b);
  EXPECT_EQ(cl8b, l8b);

  h8b = gatl::utility::number::part::second<>(v16b);
  EXPECT_EQ(ch8b, h8b);

  v16b = gatl::utility::number::part::combine<>(l8b, h8b);
  EXPECT_EQ(c16b, v16b);


  l16b = gatl::utility::number::part::first<uint16_t>(v32b);
  EXPECT_EQ(cl16b, l16b);

  l8b = gatl::utility::number::part::first<>(l16b);
  EXPECT_EQ(0x21, l8b);

  h8b = gatl::utility::number::part::second<>(l16b);
  EXPECT_EQ(0x43, h8b);

  v16b = gatl::utility::number::part::combine<>(l8b, h8b);
  EXPECT_EQ(l16b, v16b);

  h16b = gatl::utility::number::part::second<uint16_t>(v32b);
  EXPECT_EQ(ch16b, h16b);

  l8b = gatl::utility::number::part::first<>(h16b);
  EXPECT_EQ(0x65, l8b);

  h8b = gatl::utility::number::part::second<>(h16b);
  EXPECT_EQ(0x87, h8b);

  v16b = gatl::utility::number::part::combine<>(l8b, h8b);
  EXPECT_EQ(h16b, v16b);

  v32b = gatl::utility::number::part::combine<uint16_t, uint32_t>(l16b, h16b);
  EXPECT_EQ(c32b, v32b);


  l32b = gatl::utility::number::part::first<uint32_t>(v64b);
  EXPECT_EQ(c64bl, l32b);

  h32b = gatl::utility::number::part::second<uint32_t>(v64b);
  EXPECT_EQ(c64bh, h32b);

  v64b = gatl::utility::number::part::combine<uint32_t, uint64_t>(l32b, h32b);
  EXPECT_EQ(c64b, v64b);


  l16b = gatl::utility::number::part::first<uint16_t>(fpv);
  EXPECT_EQ(0x3c08, l16b);

  h16b = gatl::utility::number::part::second<uint16_t>(fpv);
  EXPECT_EQ(0x0001, h16b);

  fpv = gatl::utility::number::part::combine<uint16_t, T>(l16b, h16b);
  GOS_ARDUINO_TESTING_EQ_FP(cfpv, fpv);
}
