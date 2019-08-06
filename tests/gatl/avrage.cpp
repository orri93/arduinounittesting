#include <vector>
#include <numeric>
#include <mutex>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gos/utils/random.h>
#include <gos/utils/statistics.h>
#include <gos/utils/expect.h>

#include <gatlavrage.h>

namespace gatl = ::gos::atl;
namespace gatu = ::gos::arduino::testing::utils;

typedef std::vector<float> FloatVector;
typedef std::vector<double> DoubleVector;
typedef std::vector<uint16_t> WordVector;

static std::mutex mutext;

TEST(GatlAvrageTest, Avrage) {
  const size_t count = 256;
  DoubleVector dv;
  gatl::statistics::Set<double, uint16_t> set(NAN, count);
  gatl::statistics::Avrage<double, uint16_t> avrage(set);
  gatu::random::generate<double, uint16_t>(dv, avrage, count, 0, 1024);
  std::sort(dv.begin(), dv.end());
  double calculated, avragevalue;
  calculated = std::accumulate(dv.begin(), dv.end(), 0.0) / set.Count;
  avragevalue = avrage.get();
  EXPECT_DOUBLE_EQ(calculated, avragevalue);
  set.cleanup();
}

TEST(GatlAvrageTest, RunningAvrage) {
  mutext.lock();
  const size_t size = 16;
  const size_t count = 256;
  DoubleVector dv;
  gatu::statistics::running::vector<double, uint16_t> rv(size, dv);
  gatl::statistics::Set<double, uint16_t> set(NAN, size);
  gatl::statistics::Avrage<double, uint16_t> avrage(set);
  randomSeed(93);
  for (size_t i = 0; i < count; i++) {
    double r = gatu::random::generate<double>(0, 1024);
    rv.add(r);
    avrage.add(r);
    gatu::expect::doubleeq(dv, set);
    double calculated = std::accumulate(dv.begin(), dv.end(), 0.0) / set.Count;
    double avragevalue = avrage.get();
    EXPECT_DOUBLE_EQ(calculated, avragevalue);
  }
  set.cleanup();
  mutext.unlock();
}

