#ifndef _GOS_ARDUINO_TESTING_UTILS_STATISTICS_H_
#define _GOS_ARDUINO_TESTING_UTILS_STATISTICS_H_

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include <Arduino.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {
namespace statistics {

template<typename T> T median(const std::vector<T>& vector) {
  std::vector<T> copy(vector);
  std::sort(copy.begin(), copy.end());
  if (copy.size() > 0) {
    if (copy.size() % 2) {
      return copy.at(copy.size() / 2);
    }
    else {
      return (copy.at(copy.size() / 2) +
        copy.at(-1 + copy.size() / 2)) / 2.0;
    }
  }
  return T();
}

namespace running {
template<typename T, typename S = uint8_t>
void add(std::vector<T>& vector, const T& value, const S& size) {
  if (vector.size() == size) {
    vector.erase(vector.cbegin());
  }
  vector.push_back(value);
}

template<typename T, typename S = uint8_t>
class vector {
public:
  vector(const S& size, std::vector<T>& vector) :
    size_(size),
    index_(size),
    vector_(vector) {
  }

  void add(const T& value) {
    index_ = index_ < size_ - 1 ? index_ + 1 : 0;
    S size = static_cast<S>(vector_.size());
    if (size >= size_) {
      vector_[static_cast<std::vector<T>::size_type>(index_)] = value;
    } else {
      vector_.push_back(value);
    }
  }

private:
  S size_;
  S index_;
  std::vector<T>& vector_;
};

}

}
}
}
}
}


#endif /*_GOS_ARDUINO_TESTING_UTILS_STATISTICS_H_*/
