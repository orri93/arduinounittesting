#include <gtest/gtest.h>

#include <Arduino.h>

#include <FixedPointsCommon.h>

#include <gatltype.h>
#include <gatlutility.h>

#include <gos/utils/expect.h>

namespace fp = ::FixedPoints;
namespace gatl = ::gos::atl;
namespace gatu = ::gos::arduino::testing::utils;

TEST(GatlUtilTest, Restrict) {
  typedef fp::SQ15x16 T;

  const T minimum = -12.34;
  const T maximum = 23.45;
  const T inside = 1.23;
  const T below = -12.345;
  const T above = 23.456;

  T restricted;

  restricted = gatl::utility::restrict(inside, minimum, maximum);
  gatu::expect::fpeq(inside, restricted);

  restricted = gatl::utility::restrict(minimum, minimum, maximum);
  gatu::expect::fpeq(minimum, restricted);

  restricted = gatl::utility::restrict(maximum, minimum, maximum);
  gatu::expect::fpeq(maximum, restricted);

  restricted = gatl::utility::restrict(below, minimum, maximum);
  gatu::expect::fpeq(minimum, restricted);

  restricted = gatl::utility::restrict(above, minimum, maximum);
  gatu::expect::fpeq(maximum, restricted);

  gatl::type::range<T> range = gatl::type::make_range(minimum, maximum);

  restricted = gatl::utility::restrict(inside, range);
  gatu::expect::fpeq(inside, restricted);

  restricted = gatl::utility::restrict(minimum, range);
  gatu::expect::fpeq(minimum, restricted);

  restricted = gatl::utility::restrict(maximum, range);
  gatu::expect::fpeq(maximum, restricted);

  restricted = gatl::utility::restrict(below, range);
  gatu::expect::fpeq(minimum, restricted);

  restricted = gatl::utility::restrict(above, range);
  gatu::expect::fpeq(maximum, restricted);
}
