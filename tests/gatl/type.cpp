#include <memory>

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

TEST(GatlTypeTest, Optional) {
  typedef gatl::type::optional<int16_t> OptionalInteger;
  typedef std::unique_ptr<OptionalInteger> OptionalIntegerPointer;

  int16_t ia, ib;
  OptionalIntegerPointer oipa, oipb;
  bool b;
  
  oipa = std::make_unique<OptionalInteger>();
  EXPECT_FALSE(oipa->is());
  EXPECT_TRUE(oipa->pointer() == nullptr);
  ia = oipa->get();
  EXPECT_EQ(0, ia);

  ia = 666;
  b = oipa->isequal(ia);
  EXPECT_FALSE(b);
  b = oipa->isunequal(ia);
  EXPECT_TRUE(b);
}