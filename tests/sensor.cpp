#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <arduinosensor.h>

TEST(SensorTest, check) {
  uint8_t checkresult;
  double reading = 0.0;
  const double Minimum = -10.0;
  const double Maximum =  20.0;
  checkresult = ::gos::sensor::range::check(reading, Minimum, Maximum);
  EXPECT_EQ(GOS_SENSOR_STATUS_OK, checkresult);
  EXPECT_FLOAT_EQ(0.0, reading);
  reading = -20.0;
  checkresult = ::gos::sensor::range::check(reading, Minimum, Maximum);
  EXPECT_EQ(GOS_SENSOR_STATUS_BELOW_MINIMUM, checkresult);
  EXPECT_FLOAT_EQ(Minimum, reading);
  reading = 40.0;
  checkresult = ::gos::sensor::range::check(reading, Minimum, Maximum);
  EXPECT_EQ(GOS_SENSOR_STATUS_ABOVE_MAXIMUM, checkresult);
  EXPECT_FLOAT_EQ(Maximum, reading);
}

TEST(SensorTest, error) {
  const char* error = nullptr;
  error = ::gos::sensor::error(GOS_SENSOR_STATUS_OK);
  EXPECT_STREQ("Ok", error);
}

#ifdef GOS_SENSOR_GLOBAL
TEST(SensorTest, global) {
  uint8_t checkresult;
  double reading = 0.0;
  ::gos::sensor::range::Minimum = 10.0;
  ::gos::sensor::range::Maximum = 20.0;
  checkresult = ::gos::sensor::range::check(reading);
  EXPECT_EQ(GOS_SENSOR_STATUS_OK, checkresult);
  EXPECT_FLOAT_EQ(0.0, reading);
  reading = -20.0;
  checkresult = ::gos::sensor::range::check(reading);
  EXPECT_EQ(GOS_SENSOR_STATUS_BELOW_MINIMUM, checkresult);
  EXPECT_FLOAT_EQ(::gos::sensor::range::Minimum, reading);
  reading = 40.0;
  checkresult = ::gos::sensor::range::check(reading);
  EXPECT_EQ(GOS_SENSOR_STATUS_ABOVE_MAXIMUM, checkresult);
  EXPECT_FLOAT_EQ(::gos::sensor::range::Maximum, reading);
}
#endif