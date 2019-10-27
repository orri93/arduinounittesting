#ifndef _GOS_ARDUINO_TESTING_UTILS_MEMORY_H_
#define _GOS_ARDUINO_TESTING_UTILS_MEMORY_H_

#include <cstdint>

#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include <FixedPoints/SFixed.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {
namespace memory {

typedef ::FixedPoints::SQ15x16 FixedPointType;

typedef ::std::unique_ptr<uint8_t[]> Uint8Array;
typedef ::std::unique_ptr<uint16_t[]> Uint16Array;
typedef ::std::unique_ptr<bool[]> BooleanArray;
typedef ::std::unique_ptr<FixedPointType[]> FixedPointArray;
typedef ::std::unique_ptr<float[]> FloatArray;
typedef ::std::unique_ptr<int32_t[]> Int32Array;

void create(Uint8Array& ui8array, const uint8_t& count);
void create(Uint16Array& ui16array, const uint8_t& count);
void create(BooleanArray& boolarray, const uint8_t& count);
void create(Uint16Array& intarray, const uint8_t& count);
void create(FixedPointArray& fparray, const uint8_t& count);
void create(FloatArray& floatarray, const uint8_t& count);
void create(Int32Array& int32array, const uint8_t& count);
void transform(BooleanArray& boolarray, const uint8_t& count);

namespace pattern {
void truefalse(
  BooleanArray& boolarray,
  const uint8_t& count,
  const bool& first = true);

template<typename T, typename I = uint8_t>
void range(std::unique_ptr<T[]>& tarray, const I& count, const T& first = T()) {
  T value = first;
  for (I i = 0; i < count; i++) {
    tarray[i] = value++;
  }
}

template<typename T, typename I = uint8_t>
void decimal(
  std::unique_ptr<T[]>& tarray,
  const I& count,
  const double& first = 0.01,
  const bool& cast = false) {
  for (I i = 0; i < count; i++) {
    double id = static_cast<double>(i);
    double value = first + id + id / 10.0;
    if (cast) {
      tarray[i] = static_cast<T>(value);
    } else {
      tarray[i] = value;
    }
  }
}


template<typename T, typename I = uint8_t>
void reverse(std::unique_ptr<T[]>& typearray, const I& count) {
  std::unique_ptr<T[]> destination = std::make_unique<T[]>(count);
  reverse<T, I>(destination, typearray, count);
  for (I i = 0; i < count; i++) {
    typearray[i] = destination[i];
  }
}

template<typename T, typename I = uint8_t>
void reverse(
  std::unique_ptr<T[]>& destination,
  const std::unique_ptr<T[]>& source,
  const I& count) {
  I rc = count;
  for (I i = 0; i < count; i++) {
    destination[i] = source[--rc];
  }
}

}

namespace address {
template<typename T, typename A>
bool issame(const T* a, const T* b) {
  A aa = reinterpret_cast<A>(a);
  A ab = reinterpret_cast<A>(b);
  return (aa == ab) && (*a == *b);
}
}

}
}
}
}
}

#endif /*_GOS_ARDUINO_TESTING_UTILS_MEMORY_H_*/
