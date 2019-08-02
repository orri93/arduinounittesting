#include <vector>
#include <algorithm>

#include <Arduino.h>

#include <gos/atl/sort.h>
#include <gos/atl/median.h>

template<typename T>
void generate(
  std::vector<T>& vector,
  T* array,
  const size_t& count,
  const long& mininum,
  const long& maximum,
  const unsigned long& seed = 11ul) {
  randomSeed(seed);
  for (size_t i = 0; i < count; i++) {
    long r = random(mininum, maximum);
    vector.push_back(static_cast<T>(r));
    array[i] = static_cast<T>(r);
  }
}

template<typename T>
void generate(
  std::vector<T>& vector,
  ::gos::atl::Median<T>& median,
  const size_t& count,
  const long& mininum,
  const long& maximum,
  const unsigned long& seed = 11ul) {
  randomSeed(seed);
  for (size_t i = 0; i < count; i++) {
    long r = random(mininum, maximum);
    vector.push_back(static_cast<T>(r));
    median.add(static_cast<T>(r));
  }
}

template<typename T>
void expecteq(
  std::vector<T>& vector,
  T* array,
  const size_t& count) {
  size_t i;
  EXPECT_EQ(count, vector.size());
  std::vector<T>::iterator it = vector.begin();
  for (size_t i = 0; i < count && it != vector.end(); i++) {
    EXPECT_EQ(array[i], *(it++));
  }
  EXPECT_EQ(i, count);
  EXPECT_EQ(it, vector.end());
}
void expectfloateq(
  std::vector<float>& vector,
  float* array,
  const size_t& count) {
  size_t i;
  EXPECT_EQ(count, vector.size());
  std::vector<float>::iterator it = vector.begin();
  for (size_t i = 0; i < count && it != vector.end(); i++) {
    EXPECT_FLOAT_EQ(array[i], *(it++));
  }
  EXPECT_EQ(i, count);
  EXPECT_EQ(it, vector.end());
}
void expectfloateq(
  std::vector<float>& vector,
  float* array,
  const uint16_t* reference,
  const size_t& count) {
  size_t i;
  EXPECT_EQ(count, vector.size());
  std::vector<float>::iterator it = vector.begin();
  for (size_t i = 0; i < count && it != vector.end(); i++) {
    EXPECT_FLOAT_EQ(array[reference[i]], *(it++));
  }
  EXPECT_EQ(i, count);
  EXPECT_EQ(it, vector.end());
}

typedef std::vector<float> FloatVector;
typedef std::vector<double> DoubleVector;
typedef std::vector<uint16_t> WordVector;

TEST(ArduinoTemplateTest, Sort) {
  const size_t count = 256;
  FloatVector fv;
  float fa[count];
  generate<float>(fv, fa, count, 0, 1024);
  expectfloateq(fv, fa, count);
  ::gos::atl::sort::insertion<float>(fa, count);
  std::sort(fv.begin(), fv.end());
  expectfloateq(fv, fa, count);
  for (size_t i = 1; i < count; i++) {
    EXPECT_TRUE(fa[i - 1] <= fa[i]);
  }
}

TEST(ArduinoTemplateTest, ReferenceSort) {
  const size_t count = 256;
  FloatVector fv;
  float fa[count];
  generate<float>(fv, fa, count, 0, 1024);
  expectfloateq(fv, fa, count);
  uint16_t reference[count];
  for (size_t i = 0; i < count; i++) {
    reference[i] = static_cast<uint16_t>(i);
  }
  ::gos::atl::sort::insertion<float, uint16_t>(fa, reference, count);
  std::sort(fv.begin(), fv.end());
  expectfloateq(fv, fa, reference, count);
}

TEST(ArduinoTemplateTest, Median) {
  const size_t count = 256;
  DoubleVector dv;
  ::gos::atl::Median<double> median(NAN);
  generate<double>(dv, median, count, 0, 1024);
  std::sort(dv.begin(), dv.end());
  double calculated, medianvalue;
  if (dv.size() % 2) {
    calculated = dv.at(dv.size() / 2);
  } else {
    calculated = (dv.at(dv.size() / 2) + dv.at(-1 + dv.size() / 2)) / 2.0;
  }
  medianvalue = median.get();
  EXPECT_DOUBLE_EQ(calculated, medianvalue);
}