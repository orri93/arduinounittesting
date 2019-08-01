#ifndef _GOS_ARDUINO_TESTING_UTILS_WORD_QUEUE_H_
#define _GOS_ARDUINO_TESTING_UTILS_WORD_QUEUE_H_

#include <cstdint>
#include <queue>

#include <gos/utils/order.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {

class WordQueue {
public:
  WordQueue(const byte::Order& order = byte::Order::BigEndian);

  size_t bytes();
  size_t words();

  void pushbyte(const uint8_t& byte);
  void pushbytes(const uint8_t* data, const size_t& count);
  void pushword(const uint16_t& word);
  void pushwords(const uint16_t* words, const size_t& count);

  bool popbyte(uint8_t& value);
  bool popword(uint16_t& value);

  void clear();

private:
  typedef std::queue<uint8_t> ByteQueue;
  ByteQueue queue_;
  byte::Order order_;
};

}
}
}
}

#endif
