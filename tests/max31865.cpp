#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gosmax31865.h>

TEST(Max31865Test, error) {
  const char* error = nullptr;
  uint8_t length;
  error = ::gos::sensor::error(GOS_SENSOR_STATUS_OK, length);
  EXPECT_STREQ("Ok", error);
  EXPECT_EQ(3, length);
}
