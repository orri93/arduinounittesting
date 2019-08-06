#include <memory>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gossort.h>

#define MAXIMUM_SIZE 255

template<typename T> class Sorter {
public:
  virtual ~Sorter() {}
  virtual void sort(T* array, const uint8_t& size) = 0;
  virtual void sortref(const T* array, uint8_t* ref, const uint8_t& size) = 0;
};

class FloatSorter : public virtual Sorter<float> {
public:
  void sort(float* array, const uint8_t& size) {
    ::gos::sort::real::insertion(array, size);
  }
  void sortref(const float* array, uint8_t* ref, const uint8_t& size) {
    ::gos::sort::real::reference::insertion(array, ref, size);
  }
};

template<typename T>
int vector2array(const std::vector<T>& vector, std::unique_ptr<T[]>& array) {
  int size = static_cast<int>(vector.size());
  array.reset(new T[size]);
  EXPECT_TRUE(array);
  for (int i = size - 1; i >= 0; i--) {
    array[i] = vector.at(i);
  }
  return size;
}

template<typename T>
void vector2arrayandref(const std::vector<T>& vector, std::unique_ptr<T[]>& array, std::unique_ptr<uint8_t[]>& ref) {
  int size = vector2array(vector, array);
  ref.reset(new uint8_t[size]);
  ASSERT_TRUE(ref);
  for (int i = size - 1; i >= 0; i--) {
    ref[i] = static_cast<uint8_t>(i);
  }
}

template<typename T>
void compare(const std::vector<T>& vector, const T* array) {
  for (int i = (static_cast<int>(vector.size()) - 1); i >= 0; i--) {
    ASSERT_FLOAT_EQ(vector.at(i), array[i]);
  }
}

template<typename T>
void compareref(const std::vector<T>& vector, const T* array, const uint8_t* ref) {
  for (int i = (static_cast<int>(vector.size()) - 1); i >= 0; i--) {
    ASSERT_FLOAT_EQ(vector.at(i), array[ref[i]]);
  }
}

template<typename T> void testsort(Sorter<T>& sorter) {
  std::vector<T> v;
  std::unique_ptr<T[]> a;
  for (int i = 0; i < MAXIMUM_SIZE; i++) {
    T r = (T)(rand() % 255 + 1);
    v.push_back(r);
    vector2array(v, a);
    std::vector<T> c(v);
    std::sort(c.begin(), c.end());
    sorter.sort(a.get(), static_cast<uint8_t>(v.size()));
    compare(c, a.get());
  }
}

template<typename T> void testsortref(Sorter<T>& sorter) {
  std::vector<T> v;
  std::unique_ptr<T[]> a;
  std::unique_ptr<uint8_t[]> ref;
  for (int i = 0; i < MAXIMUM_SIZE; i++) {
    T r = (T)(rand() % 255 + 1);
    v.push_back(r);
    vector2arrayandref(v, a, ref);
    std::vector<T> c(v);
    std::sort(c.begin(), c.end());
    sorter.sortref(a.get(), ref.get(), static_cast<uint8_t>(v.size()));
    compareref(c, a.get(), ref.get());
  }
}

TEST(SortTest, InsertionSortFloats) {
  ::srand(93);
  FloatSorter sorter;
  testsort(sorter);
}

TEST(SortTest, InsertionSortFloatReferences) {
  ::srand(93);
  FloatSorter sorter;
  testsortref(sorter);
}
