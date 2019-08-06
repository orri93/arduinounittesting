#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlcrc.h>

#define GATL_CRC_TEST_TEXT "Þetta er texti til prófunar á CRC"

TEST(GatlCrcTest, Calculation) {
  const uint32_t ccrc = 0xcc9448d7;

  uint32_t crca, crcb;
  crca = ::gos::atl::crc::calculate<uint32_t, size_t, uint8_t>(
    static_cast<const void*>(GATL_CRC_TEST_TEXT),
    sizeof(GATL_CRC_TEST_TEXT));
  EXPECT_EQ(ccrc, crca);

  uint8_t buffer[sizeof(GATL_CRC_TEST_TEXT)];
  memcpy(buffer, GATL_CRC_TEST_TEXT, sizeof(GATL_CRC_TEST_TEXT));
  crcb = ::gos::atl::crc::calculate<uint32_t, size_t, uint8_t>(
    buffer, sizeof(GATL_CRC_TEST_TEXT));
  EXPECT_EQ(ccrc, crcb);
  EXPECT_EQ(crca, crcb);

  buffer[sizeof(GATL_CRC_TEST_TEXT) / 2] = 'X';
  crcb = ::gos::atl::crc::calculate<uint32_t, size_t, uint8_t>(
    buffer, sizeof(GATL_CRC_TEST_TEXT));
  EXPECT_NE(crca, crcb);
}