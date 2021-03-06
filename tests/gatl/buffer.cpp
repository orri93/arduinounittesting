#include <memory>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlbuffer.h>

#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include <FixedPoints/SFixed.h>

#include <gos/utils/expect.h>
#include <gos/utils/memory.h>
#include <gos/utils/binding.h>

#define CRC_LENGTH 2

#define READ_UINT16(arr, index) word(arr[index], arr[index + 1])
#define READ_READCRC(arr, length) word(arr[(length - CRC_LENGTH) \
  + 1], arr[length - CRC_LENGTH])

/*                   123456789012345678901234 */
#define TEST_BUFFER "ABCDEFGHIJKLMNO123456789"

namespace ga = ::gos::atl;
namespace gab = ::gos::atl::buffer;

typedef gab::Holder<uint16_t, uint8_t> CharBuffer;

uint64_t sum(CharBuffer& buffer) {
  uint64_t s = 0;
  for (uint16_t i = 0; i < buffer.Size; ++i) {
    uint8_t b = static_cast<uint8_t>(buffer.Buffer[i]);
    s += b;
  }
  return s;
}

TEST(GatlBufferTest, Construct) {
  CharBuffer buffer(32);

  EXPECT_EQ(32, buffer.Size);
}

TEST(GatlBufferTest, Clear) {
  uint64_t s;
  CharBuffer buffer(32);
  
  gab::clear(buffer);
  s = sum(buffer);
  EXPECT_EQ(0, s);

  ::memcpy(buffer.Buffer, TEST_BUFFER, sizeof(TEST_BUFFER));
  s = sum(buffer);
  EXPECT_EQ(1557, s);

  gab::clear(buffer);
  s = sum(buffer);
  EXPECT_EQ(0, s);
}

TEST(GatlBufferTest, Read) {
  uint64_t s;
  uint16_t w, crc;

  CharBuffer buffer(32);

  gab::clear(buffer);

  s = sum(buffer);
  EXPECT_EQ(0, s);

  ::memcpy(buffer.Buffer, TEST_BUFFER, sizeof(TEST_BUFFER));
  s = sum(buffer);
  EXPECT_EQ(1557, s);

  w = READ_UINT16(buffer.Buffer, 2);
  EXPECT_EQ(0x4344, w);

  w = READ_UINT16(buffer.Buffer, 4);
  EXPECT_EQ(0x4546, w);

  crc = READ_READCRC(buffer.Buffer, 6);
  EXPECT_EQ(0x4645, crc);
}

TEST(GatlBufferTest, Bitwise) {
  uint16_t i;
  uint64_t s;

  CharBuffer buffer(32);
  
  gab::clear(buffer);

  s = sum(buffer);
  EXPECT_EQ(0, s);


  bitSet(buffer.Buffer[2], 2);
  s = sum(buffer);
  EXPECT_EQ(4, s);

  buffer.Buffer[5] = static_cast<char>(0xff);
  s = sum(buffer);
  EXPECT_EQ(4 + 0xff, s);

  i = 6;
  bitClear(buffer.Buffer[i - 1], 6);
  s = sum(buffer);
  EXPECT_EQ(4 + 0xbf, s);
}