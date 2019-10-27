#include <gos/utils/memory.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {

namespace memory {
void create(Uint8Array& ui8array, const uint8_t& count) {
  ui8array = std::make_unique<uint8_t[]>(count);
  for (size_t i = 0; i < count; i++) {
    ui8array[i] = 0;
  }
}

void create(Uint16Array& ui16array, const uint8_t& count) {
  ui16array = std::make_unique<uint16_t[]>(count);
  for (size_t i = 0; i < count; i++) {
    ui16array[i] = 0;
  }
}

void create(BooleanArray& boolarray, const uint8_t& count) {
  boolarray = std::make_unique<bool[]>(count);
  for (size_t i = 0; i < count; i++) {
    boolarray[i] = false;
  }
}

void create(FixedPointArray& fparray, const uint8_t& count) {
  fparray = std::make_unique<FixedPointType[]>(count);
  for (size_t i = 0; i < count; i++) {
    fparray[i] = 0.0;
  }
}

void create(FloatArray& floatarray, const uint8_t& count) {
  floatarray = std::make_unique<float[]>(count);
  for (size_t i = 0; i < count; i++) {
    floatarray[i] = 0.0F;
  }
}

void create(Int32Array& int32array, const uint8_t& count) {
  int32array = std::make_unique<int32_t[]>(count);
  for (size_t i = 0; i < count; i++) {
    int32array[i] = 0;
  }
}

void transform(BooleanArray& boolarray, const uint8_t& count) {
  for (size_t i = 0; i < count; i++) {
    boolarray[i] = boolarray[i] ? false : true;
  }
}
namespace pattern {
void truefalse(
  BooleanArray& boolarray,
  const uint8_t& count,
  const bool& first) {
  bool value = first;
  for (uint8_t i = 0; i < count; i++) {
    boolarray[i] = value;
    value = value ? false : true;
  }
}
}
}

}
}
}
}
