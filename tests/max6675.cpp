#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gosmax6675.h>

TEST(Max6675Test, error) {
  const char* error = nullptr;
  uint8_t length;
  double value;
  bool readresult;
  ::gos::Max6675 max6675;
  max6675.initialize();
  readresult = max6675.read(value);
  EXPECT_TRUE(readresult);
  //EXPECT_STREQ("Ok", error);
  EXPECT_DOUBLE_EQ(0.0, value);
  error = max6675.error(length);
  EXPECT_TRUE(error == nullptr);
  EXPECT_EQ(0, length);
}
