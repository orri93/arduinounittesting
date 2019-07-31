#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gosmax31865.h>
#include <arduinosensor.h>

TEST(Max31865Test, error) {
  const char* error = nullptr;
  uint8_t length;
  double value;
  bool readresult;
  ::gos::Max31865 max31865;
  max31865.initialize();
  readresult = max31865.read(value);
  error = max31865.error(length);
  EXPECT_TRUE(error == nullptr);
  EXPECT_EQ(0, length);

}
