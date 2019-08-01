#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Arduino.h>
#include <SPI.h>

#include <gosmax6675.h>

TEST(Max6675Test, Max6675) {
  ArduinoMock* arduinomock = arduinoMockInstance();
  SpiMock* spimock = spiMockInstance();

  spimock->Out.pushword(0x0004);
  spimock->Out.pushword(0x0400);

  const uint8_t pin = 10;
  const double presetvalue = -99.99;
  const char* error = nullptr;
  uint8_t length;
  double value = presetvalue;
  bool readresult;
  ::gos::Max6675 max6675(pin);
  EXPECT_CALL(*arduinomock, pinMode(pin, OUTPUT)).
    Times(testing::Exactly(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, HIGH)).
    Times(testing::AtLeast(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, LOW)).
    Times(testing::AtLeast(1));
  max6675.initialize();
  readresult = max6675.read(value);
  EXPECT_FALSE(readresult);
  EXPECT_DOUBLE_EQ(presetvalue, value);
  error = max6675.error(length);
  EXPECT_STREQ("OPEN", error);
  EXPECT_EQ(5, length);
  readresult = max6675.read(value);
  EXPECT_TRUE(readresult);
  EXPECT_DOUBLE_EQ(32.0, value);
  error = max6675.error(length);
  EXPECT_TRUE(error == nullptr);
  EXPECT_EQ(0, length);

  releaseSpiMock();
  releaseArduinoMock();
}
