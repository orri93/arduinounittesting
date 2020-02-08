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

typedef gab::Holder<uint16_t, char> CharBuffer;

uint64_t sum(CharBuffer& buffer) {
  uint64_t s = 0;
  for (uint16_t i = 0; i < buffer.Size; ++i) {
    s += buffer.Buffer[i];
  }
  return s;
}

TEST(GatlBufferTest, Construct) {
  CharBuffer buffer(32);

  EXPECT_EQ(32, buffer.Size);
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
