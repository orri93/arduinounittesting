#ifndef _GOS_ARDUINO_TESTING_UTILS_RANDOM_H_
#define _GOS_ARDUINO_TESTING_UTILS_RANDOM_H_

#include <Arduino.h>

#include <gatlmedian.h>
#include <gatlavrage.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {
namespace random {

template<typename T>
T generate(
  const long& minimum,
  const long& maximum) {
  return static_cast<T>(::random(minimum, maximum));
}

template<typename T>
void generate(
  std::vector<T>& vector,
  T* array,
  const size_t& count,
  const long& minimum,
  const long& maximum,
  const unsigned long& seed = 11ul) {
  randomSeed(seed);
  for (size_t i = 0; i < count; i++) {
    T r = generate<T>(minimum, maximum);
    vector.push_back(r);
    array[i] = r;
  }
}

template<typename T, typename I>
void generate(
  std::vector<T>& vector,
  ::gos::atl::statistics::Set<T, I>& set,
  const size_t& count,
  const long& minimum,
  const long& maximum,
  const unsigned long& seed = 11ul) {
  randomSeed(seed);
  for (size_t i = 0; i < count; i++) {
    T r = generate<T>(minimum, maximum);
    vector.push_back(r);
    set.add(r);
  }
}

template<typename T, typename I>
void generate(
  std::vector<T>& vector,
  ::gos::atl::statistics::Avrage<T, I>& avrage,
  const size_t& count,
  const long& minimum,
  const long& maximum,
  const unsigned long& seed = 11ul) {
  randomSeed(seed);
  for (size_t i = 0; i < count; i++) {
    T r = generate<T>(minimum, maximum);
    vector.push_back(r);
    avrage.add(r);
  }
}

}
}
}
}
}

#endif /*_GOS_ARDUINO_TESTING_UTILS_RANDOM_H_*/
