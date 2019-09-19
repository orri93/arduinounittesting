#include <gtest/gtest.h>

#include <Arduino.h>

#include <FixedPointsCommon.h>

#include <gatltype.h>

#include <gos/utils/expect.h>

namespace fp = ::FixedPoints;
namespace gatl = ::gos::atl;

TEST(GatlTypeTest, Range) {
  typedef fp::SQ15x16 T;
  const T lowest = -12.34;
  const T highest = 23.45;

  gatl::type::range<T> range = gatl::type::make_range(lowest, highest);

  GOS_ARDUINO_TESTING_EQ_FP(lowest, range.lowest);
  GOS_ARDUINO_TESTING_EQ_FP(highest, range.highest);
}
