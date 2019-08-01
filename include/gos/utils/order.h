#ifndef _GOS_ARDUINO_TESTING_UTILS_ORDER_H_
#define _GOS_ARDUINO_TESTING_UTILS_ORDER_H_

#include <cstdint>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {
namespace byte {
enum class Order {
  Undefined = 0,
  LittleEndian = 1,
  BigEndian = 2
};
Order contrary(const Order& order);
uint16_t combine(const uint8_t& first, const uint8_t& second, const Order& order = Order::BigEndian);
uint8_t fyrstbyte(const uint16_t& word, const Order& order = Order::BigEndian);
uint8_t secondbyte(const uint16_t& word, const Order& order = Order::BigEndian);
}
namespace bit {
enum class Order {
  Undefined = 0,
  MsbFirst = 1,
  LsbFirst = 2
};
}
}
}
}
}


#endif /*_GOS_ARDUINO_TESTING_UTILS_BIT_H_*/
