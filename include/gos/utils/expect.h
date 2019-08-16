#ifndef _GOS_ARDUINO_TESTING_UTILS_EXPECT_H_
#define _GOS_ARDUINO_TESTING_UTILS_EXPECT_H_

#include <Arduino.h>

#include <gtest/gtest.h>

#include <gatlstatistics.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {
namespace expect {

template<typename T>
void eq(
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

template<typename T, typename I>
void eq(
  std::vector<T>& vector,
  const ::gos::atl::statistics::Set<T, I>& set) {
  I i;
  EXPECT_EQ(set.Count, vector.size());
  std::vector<T>::const_iterator it = vector.cbegin();
  for (i = 0; it != vector.cend() && i < set.Count; i++) {
    EXPECT_EQ(*(it++), set.Values[i]);
  }
  EXPECT_EQ(i, vector.size());
  EXPECT_EQ(it, vector.end());
}

template<typename I>
void doubleeq(
  const std::vector<double>& vector,
  const ::gos::atl::statistics::Set<double, I>& set) {
  I i;
  EXPECT_EQ(set.Count, vector.size());
  std::vector<double>::const_iterator it = vector.cbegin();
  for (i = 0; it != vector.cend() && i < set.Count; i++) {
    EXPECT_EQ(*(it++), set.Values[i]);
  }
  EXPECT_EQ(i, vector.size());
  EXPECT_EQ(it, vector.end());
}

template<typename FPQ> void fpeq(const FPQ& a, const FPQ& b) {
  EXPECT_EQ(a.getInteger(), b.getInteger());
  EXPECT_EQ(a.getFraction(), b.getFraction());
}

template<typename FPQ> void fpnear(const FPQ& a, const FPQ& b, const FPQ& f) {
  EXPECT_TRUE((a - b) * (a - b) < f * f);
}

void floateq(
  std::vector<float>& vector,
  float* array,
  const size_t& count);
void floateq(
  std::vector<float>& vector,
  float* array,
  const uint16_t* reference,
  const size_t& count);

}
}
}
}
}


#endif /*_GOS_ARDUINO_TESTING_UTILS_EXPECT_H_*/