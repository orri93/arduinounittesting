#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fdsmedian.h>

TEST(MedianTest, add) {
  fds::statistics::Median median;
  median.add(12.32);
  median.add(33.44);
  float m = median.median();
  EXPECT_FLOAT_EQ(22.0F, m);
}