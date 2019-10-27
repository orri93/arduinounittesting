#ifndef _GOS_ARDUINO_TESTING_UTILS_BINDING_H_
#define _GOS_ARDUINO_TESTING_UTILS_BINDING_H_

#include <cstdint>

#include <memory>

#include <gatlbinding.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {
namespace binding {

template<typename T, typename U, typename S = size_t> S size() {
  return sizeof(T) / sizeof(U);
}

template<typename T, typename A = uint16_t, typename I = uint8_t>
A create(
  ::gos::atl::binding::reference<T, A, I>& binding,
  ::std::unique_ptr<T[]>& pointers,
  const A& start,
  const I& count,
  const I& size) {
  A result = A();
  if (pointers) {
    T* pointer = pointers.get();
    result = ::gos::atl::binding::create<T, A, I>(binding, start, count, size);
    for (I i = 0; i < count; i++) {
      ::gos::atl::binding::set(binding, i, pointer++);
    }
  }
  return result;
}

template<typename T, typename A = uint16_t, typename I = uint8_t>
A createtwo(
  ::gos::atl::binding::reference<T, A, I>& binding,
  const A& address,
  const I& size,
  T* one,
  T* two) {
  A result = ::gos::atl::binding::create<T, A, I>(binding, address, 2, size);
  ::gos::atl::binding::set<T, A, I>(binding, 0, one);
  ::gos::atl::binding::set<T, A, I>(binding, 1, two);
  return result;
}

template<typename T, typename A = uint16_t, typename I = uint8_t>
A createthree(
  ::gos::atl::binding::reference<T, A, I>& binding,
  const A& address,
  const I& size,
  T* one,
  T* two,
  T* three) {
  uint8_t index = 0;
  A result = ::gos::atl::binding::create<T, A, I>(binding, address, 3, size);
  ::gos::atl::binding::set<T, A, I>(binding, 0, one);
  ::gos::atl::binding::set<T, A, I>(binding, 1, two);
  ::gos::atl::binding::set<T, A, I>(binding, 2, three);
  return result;
}

template<typename T, typename A = uint16_t, typename I = uint8_t>
A createfour(
  ::gos::atl::binding::reference<T, A, I>& binding,
  const A& address,
  const I& size,
  T* one,
  T* two,
  T* three,
  T* four) {
  A result = ::gos::atl::binding::create<T, A, I>(binding, address, 4, size);
  ::gos::atl::binding::set<T, A, I>(binding, 0, one);
  ::gos::atl::binding::set<T, A, I>(binding, 1, two);
  ::gos::atl::binding::set<T, A, I>(binding, 2, three);
  ::gos::atl::binding::set<T, A, I>(binding, 3, four);
  return result;
}

}
}
}
}
}

#endif /*_GOS_ARDUINO_TESTING_UTILS_BINDING_H_ */
