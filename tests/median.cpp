#include <cstdlib>
#include <algorithm>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fdsmedian.h>

typedef std::vector<float> FloatVector;
typedef FloatVector::iterator FloatVectorIterator;
typedef FloatVector::size_type FloatVectorSize;

const float A = 12.32F;
const float B = 33.44F;
const float C = 93.11F;
const float D = 666.44F;

const float M1 = A;
const float M2 = 22.879999F;
const float M3 = 93.110001F;

namespace mu = std;

static float vmedian(std::vector<float> vector) {
  int size = static_cast<int>(vector.size());
  if (size > 2) {
    std::sort(vector.begin(), vector.end());
  }
  switch (size) {
  case 0:
    return NAN;
  case 1:
    return vector.at(0);
  case 2:
    return (vector.at(0) + vector.at(1)) / 2.0F;
  case 3:
    return vector.at(1);
  case 4:
    return (vector.at(1) + vector.at(2)) / 2.0F;
  default:
    int half = size / 2;
    if (size % 2 == 0) {
      return (vector.at(half) + vector.at(half - 1)) / 2.0F;
    }
    else {
      return vector.at(half);
    }
  }
}

static void addtwo(fds::statistics::Median& median) {
  median.add(A);
  median.add(B);
}

static void addthree(fds::statistics::Median& median) {
  median.add(D);
  median.add(A);
  median.add(C);
}

class TestableMedian : public fds::statistics::Median {
public:
  uint8_t count() {
    return count_;
  }

  void access(std::vector<float>& vector) {
    vector.clear();
    for (int i = 0; i < count_; i++) {
      vector.push_back(values_[i]);
    }
  }
};

TEST(MedianTest, add) {
  float m;
  fds::statistics::Median median;
  addtwo(median);
  m = median.median();
  EXPECT_FLOAT_EQ(M2, m);
}

TEST(MedianTest, median) {
  float m;
  fds::statistics::Median median;
  addthree(median);
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
}

TEST(MedianTest, reset) {
  float m;
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
  float m;
  fds::statistics::Median median;
  addthree(median);
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
  median.clearallbutlast();
  m = median.median();
  EXPECT_FLOAT_EQ(M3, m);
}

static void showvector(const char* id, FloatVector& v) {
  std::cout << id << " : ";
  FloatVectorIterator it = v.begin();
  while (it != v.end()) {
    std::cout << (it != v.begin() ? ", " : "") << *it++;
  }
  std::cout << std::endl;
}

TEST(MedianTest, comparetosortmedian) {
  float m, vm;
  TestableMedian testablemedian;
  FloatVector values, testablemedianvalues;
  ::srand(93);
  FloatVectorIterator it;
  for (int i = 0; i < 256; i++) {
    int r = rand() % 666 + 1;
    float f = static_cast<float>(r);
    //std::cout << "at " << i << std::endl;
    //std::cout << "adding " << f << std::endl;
    testablemedian.add(f);
    values.push_back(f);
    if (values.size() > MEDIAN_COUNT) {
      it = values.begin();
      values.erase(it);
    }
    testablemedian.access(testablemedianvalues);
    FloatVectorSize sizea = values.size();
    FloatVectorSize sizeb = testablemedianvalues.size();
    ASSERT_EQ(sizea, sizeb);
    FloatVectorSize count = std::min(sizea, sizeb);
    FloatVectorIterator ita = values.begin();
    FloatVectorIterator itb = testablemedianvalues.begin();
    //showvector("V", values);
    //showvector("T", testablemedianvalues);
    float suma = 0.0, sumb = 0.0;
    for (FloatVectorSize j = 0; j < count; j++) {
      suma += *ita;
      sumb += *itb;
      ita++;
      if (ita == values.end()) {
        break;
      }
      itb++;
      if (itb == testablemedianvalues.end()) {
        break;
      }
    }
    ASSERT_FLOAT_EQ(suma, sumb);
    m = testablemedian.median();
    vm = vmedian(values);
    ASSERT_FLOAT_EQ(vm, m);
  }
}