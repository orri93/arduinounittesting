#include <gos/utils/wordq.h>

namespace gatub = ::gos::arduino::testing::utils::byte;

namespace gos {
namespace arduino {
namespace testing {
namespace utils {

WordQueue::WordQueue(const byte::Order& order)
  : order_(order) {
}

size_t WordQueue::bytes() {
  return queue_.size();
}

size_t WordQueue::words() {
  return bytes() / 2;
}

void WordQueue::pushbyte(const uint8_t& byte) {
  queue_.push(byte);
}

void WordQueue::pushbytes(const uint8_t* bytes, const size_t& count) {
  for (size_t i = 0; i < count; i++) {
    pushbyte(bytes[i]);
  }
}

void WordQueue::pushword(const uint16_t& word) {
  uint8_t byte;
  byte = gatub::fyrstbyte(word, order_);
  queue_.push(byte);
  byte = gatub::secondbyte(word, order_);
  queue_.push(byte);
}

void WordQueue::pushwords(const uint16_t* words, const size_t& count) {
  for (size_t i = 0; i < count; i++) {
    pushword(words[i]);
  }
}

bool WordQueue::popbyte(uint8_t& value) {
  if (queue_.size() > 0) {
    value = queue_.front();
    queue_.pop();
    return true;
  } else {
    return false;
  }
}
 
bool WordQueue::popword(uint16_t& value) {

  if (queue_.size() > 1) {
    uint8_t first = queue_.front();
    queue_.pop();
    uint8_t second = queue_.front();
    queue_.pop();
    value = gatub::combine(first, second, order_);
    return true;
  } else {
    return false;
  }
}

void WordQueue::clear() {
  while (!queue_.empty()) {
    queue_.pop();
  }
}

}
}
}
}
