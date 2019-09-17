#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Arduino.h>

#include <FixedPoints.h>
#include <FixedPointsCommon.h>

namespace fp = ::FixedPoints;

TEST(FixedPointsTest, SizeOf) {
  typedef fp::SQ15x16 fpt;
  
  size_t size;
  char ch;

  fpt fixed = 1.234;

  size = sizeof(fpt);
  ASSERT_EQ(4, size);
  size = sizeof(fixed);
  ASSERT_EQ(4, size);

  char* pointer = reinterpret_cast<char*>(&fixed);
  ch = *pointer++;
  ASSERT_EQ('\xE7', ch);
  ch = *pointer++;
  ASSERT_EQ(';', ch);
  ch = *pointer++;
  ASSERT_EQ('\x1', ch);
  ch = *pointer++;
  ASSERT_EQ('\0', ch);
}