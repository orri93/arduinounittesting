#include <vector>
#include <mutex>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gos/utils/random.h>
#include <gos/utils/statistics.h>
#include <gos/utils/expect.h>

#include <gatlmedian.h>

namespace gatl = ::gos::atl;
namespace gatu = ::gos::arduino::testing::utils;

typedef std::vector<double> DoubleVector;
typedef std::vector<uint32_t> DoubleWordVector;

static std::mutex mutext;

TEST(GatlMedianTest, Median) {
  const size_t count = 256;
  DoubleVector dv;
  gatl::statistics::Set<double, uint16_t> set(NAN, count);
  gatl::statistics::Median<double, uint16_t> median(set);
  gatu::random::generate<double, uint16_t>(dv, set, count, 0, 1024);
  std::sort(dv.begin(), dv.end());
  double calculated, medianvalue;
  calculated = gatu::statistics::median(dv);
  medianvalue = median.get();
  EXPECT_DOUBLE_EQ(calculated, medianvalue);
  median.cleanup();
}

TEST(GatlMedianTest, RunningMedian) {
  mutext.lock();
  const size_t size = 16;
  const size_t count = 256;

  DoubleVector dv;
  gatu::statistics::running::vector<double, uint16_t> rv(size, dv);
  gatl::statistics::Set<double, uint16_t> set(NAN, size);
  gatl::statistics::Median<double, uint16_t> median(set);
  randomSeed(93);
  for (size_t i = 0; i < count; i++) {
    double r = gatu::random::generate<double>(0, 1024);
    rv.add(r);
    set.add(r);
    gatu::expect::doubleeq(dv, set);
    double calculated = gatu::statistics::median(dv);
    double medianvalue = median.get();
    EXPECT_DOUBLE_EQ(calculated, medianvalue);
  }
  median.cleanup();

  DoubleWordVector dwv;
  gatu::statistics::running::vector<uint32_t, uint8_t> rdwv(size, dwv);
  gatl::statistics::Set<uint32_t, uint8_t> dwset(0xffffffff, size);
  gatl::statistics::Median<uint32_t, uint8_t> dwmedian(dwset);
  for (size_t i = 0; i < count; i++) {
    uint32_t dwr = gatu::random::generate<uint32_t>(0, 1024);
    rdwv.add(dwr);
    dwset.add(dwr);
    gatu::expect::eq<uint32_t>(dwv, dwset);
    uint32_t dwcalculated = gatu::statistics::median(dwv);
    uint32_t dwmedianvalue = dwmedian.get();
    EXPECT_EQ(dwcalculated, dwmedianvalue);
  }

  mutext.unlock();
}
