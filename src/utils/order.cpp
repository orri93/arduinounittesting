#include <gos/utils/order.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {

namespace byte {
Order contrary(const Order& order) {
  switch (order)
  {
  case Order::BigEndian:
    return Order::LittleEndian;
  case Order::LittleEndian:
    return Order::BigEndian;
  default:
    return order;
  }
}
uint16_t combine(const uint8_t& first, const uint8_t& second, const Order& order) {
  switch (order)
  {
  case Order::BigEndian:
    return (static_cast<uint16_t>(first) << 8) | static_cast<uint16_t>(second);
  case Order::LittleEndian:
    return (static_cast<uint16_t>(second) << 8) | static_cast<uint16_t>(first);
  default:
    return 0x0000;
  }
}
uint8_t fyrstbyte(const uint16_t& word, const Order& order) {
  switch (order) {
  case Order::BigEndian:
    return static_cast<uint8_t>((word & 0xff00) >> 8);
  case Order::LittleEndian:
    return static_cast<uint8_t>(word & 0x00ff);
  default:
    return 0x00;
  }
}
uint8_t secondbyte(const uint16_t& word, const Order& order) {
  return fyrstbyte(word, contrary(order));
}
}

}
}
}
}
