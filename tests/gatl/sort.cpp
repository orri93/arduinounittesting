#include <vector>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gos/utils/random.h>
#include <gos/utils/expect.h>

#include <gatlsort.h>

namespace gatl = ::gos::atl;
namespace gatu = ::gos::arduino::testing::utils;

typedef std::vector<float> FloatVector;

TEST(GatlSortTest, Sort) {
  const size_t count = 256;
  FloatVector fv;
  float fa[count];
  gatu::random::generate<float>(fv, fa, count, 0, 1024);
  GOS_ARDUINO_TESTING_EQ_VECTOR_ARRAY_FLOAT(fv, fa, count);
  //gatu::expect::floateq(fv, fa, count);
  gatl::sort::insertion<float>(fa, count);
  std::sort(fv.begin(), fv.end());
  GOS_ARDUINO_TESTING_EQ_VECTOR_ARRAY_FLOAT(fv, fa, count);
  //gatu::expect::floateq(fv, fa, count);
  for (size_t i = 1; i < count; i++) {
    EXPECT_TRUE(fa[i - 1] <= fa[i]);
  }
}

TEST(GatlSortTest, ReferenceSort) {
  const size_t count = 256;
  FloatVector fv;
  float fa[count];
  gatu::random::generate<float>(fv, fa, count, 0, 1024);
  GOS_ARDUINO_TESTING_EQ_VECTOR_ARRAY_FLOAT(fv, fa, count);
  //gatu::expect::floateq(fv, fa, count);
  uint16_t reference[count];
  for (size_t i = 0; i < count; i++) {
    reference[i] = static_cast<uint16_t>(i);
  }
  gatl::sort::insertion<float, uint16_t>(fa, reference, count);
  std::sort(fv.begin(), fv.end());
  GOS_ARDUINO_TESTING_EQ_VECTOR_ARRAY_REFERENCE_FLOAT(fv, fa, reference, count);
  //gatu::expect::floateq(fv, fa, reference, count);
}
