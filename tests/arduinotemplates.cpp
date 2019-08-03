#include <vector>
#include <numeric>
#include <mutex>

#include <Arduino.h>

#include <gos/utils/random.h>
#include <gos/utils/statistics.h>
#include <gos/utils/expect.h>

#include <gos/atl/sort.h>
#include <gos/atl/median.h>
#include <gos/atl/avrage.h>

namespace gatl = ::gos::atl;
namespace gatu = ::gos::arduino::testing::utils;

typedef std::vector<float> FloatVector;
typedef std::vector<double> DoubleVector;
typedef std::vector<uint16_t> WordVector;

std::mutex mutext;

TEST(ArduinoTemplateTest, Sort) {
  const size_t count = 256;
  FloatVector fv;
  float fa[count];
  gatu::random::generate<float>(fv, fa, count, 0, 1024);
  gatu::expect::floateq(fv, fa, count);
  gatl::sort::insertion<float>(fa, count);
  std::sort(fv.begin(), fv.end());
  gatu::expect::floateq(fv, fa, count);
  for (size_t i = 1; i < count; i++) {
    EXPECT_TRUE(fa[i - 1] <= fa[i]);
  }
}

TEST(ArduinoTemplateTest, ReferenceSort) {
  const size_t count = 256;
  FloatVector fv;
  float fa[count];
  gatu::random::generate<float>(fv, fa, count, 0, 1024);
  gatu::expect::floateq(fv, fa, count);
  uint16_t reference[count];
  for (size_t i = 0; i < count; i++) {
    reference[i] = static_cast<uint16_t>(i);
  }
  gatl::sort::insertion<float, uint16_t>(fa, reference, count);
  std::sort(fv.begin(), fv.end());
  gatu::expect::floateq(fv, fa, reference, count);
}

TEST(ArduinoTemplateTest, Median) {
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

TEST(ArduinoTemplateTest, RunningMedian) {
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
  mutext.unlock();
}

TEST(ArduinoTemplateTest, Avrage) {
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

TEST(ArduinoTemplateTest, RunningAvrage) {
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
