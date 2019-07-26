#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fdsmedian.h>

const float A = 12.32F;
const float B = 33.44F;
const float C = 93.11F;
const float D = 666.44F;

const float M1 = A;
const float M2 = 22.879999F;
const float M3 = 93.110001F;

static float m;

static void addtwo(fds::statistics::Median& median) {
  median.add(A);
  median.add(B);
}

static void addthree(fds::statistics::Median& median) {
  median.add(D);
  median.add(A);
  median.add(C);
}

TEST(MedianTest, add) {
  fds::statistics::Median median;
  addtwo(median);
  m = median.median();
  EXPECT_FLOAT_EQ(M2, m);
}

TEST(MedianTest, median) {
  fds::statistics::Median median;
  addthree(median);
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
}

TEST(MedianTest, reset) {
  fds::statistics::Median median;
  addthree(median);
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
  median.reset();
  m = median.median();
  EXPECT_TRUE(::isnan(m));
  median.add(C);
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
}

TEST(MedianTest, clearallbutlast) {
  fds::statistics::Median median;
  addthree(median);
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
  median.clearallbutlast();
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
}
