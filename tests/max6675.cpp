#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Arduino.h>
#include <SPI.h>

#include <gosmax6675.h>

TEST(Max6675Test, error) {
  ArduinoMock* arduinomock = arduinoMockInstance();
  SpiMock* spimock = spiMockInstance();

  const uint8_t pin = 10;
  const char* error = nullptr;
  uint8_t length;
  double value;
  bool readresult;
  ::gos::Max6675 max6675(pin);
  EXPECT_CALL(*arduinomock, pinMode(pin, OUTPUT)).
    Times(testing::Exactly(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, HIGH)).
    Times(testing::Exactly(1));
  max6675.initialize();
  EXPECT_CALL(*spimock, beginTransaction(SPISettings(
    SPI_CLOCK_MAX_6675,
    SPI_BIT_ORDER_MAX_6675,
    SPI_MODE_MAX_6675))).Times(testing::Exactly(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, LOW)).
    Times(testing::Exactly(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, HIGH)).
    Times(testing::Exactly(1));
  EXPECT_CALL(*spimock, endTransaction()).
    Times(testing::Exactly(1));
  EXPECT_CALL(*spimock, transfer16(0x0000)).
    Times(testing::Exactly(1));
  readresult = max6675.read(value);
  EXPECT_TRUE(readresult);
  //EXPECT_STREQ("Ok", error);
  EXPECT_DOUBLE_EQ(0.0, value);
  error = max6675.error(length);
  EXPECT_TRUE(error == nullptr);
  EXPECT_EQ(0, length);

  releaseSpiMock();
  releaseArduinoMock();
}
