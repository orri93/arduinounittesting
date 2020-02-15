#include <iostream>
#include <mutex>
#include <map>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlled.h>

namespace gatll = ::gos::atl::led;

class GatlLedFixture : public ::testing::Test {
public:
  void SetUp() override {
    arduinomock = arduinoMockInstance();
  }

  void TearDown() override {
    releaseArduinoMock();
  }

  const uint8_t Pin = 11;
  const uint8_t Count = 6;
  const unsigned long Delay = 250;

  typedef std::map<uint8_t, uint8_t> OutputCountMap;
  typedef OutputCountMap::iterator OutputCountIterator;
  typedef OutputCountMap::value_type OutputCountValue;

  ArduinoMock* arduinomock;

  std::mutex mutex;
};

TEST_F(GatlLedFixture, Initialize) {
  EXPECT_CALL(*arduinomock, pinMode(Pin, OUTPUT)).Times(::testing::Exactly(1));
  gatll::initialize(Pin);
}

TEST_F(GatlLedFixture, Blink) {
  EXPECT_CALL(*arduinomock, digitalWrite(Pin, HIGH))
    .Times(::testing::Exactly(Count));
  EXPECT_CALL(*arduinomock, digitalWrite(Pin, LOW))
    .Times(::testing::Exactly(Count));
  EXPECT_CALL(*arduinomock, delay(Delay))
    .Times(::testing::Exactly(Count * 2));
  gatll::blink(Pin, Count, Delay);
}

TEST_F(GatlLedFixture, SinInitialize) {
  double at;
  gatll::sin::initialize<double>(at);
  EXPECT_DOUBLE_EQ(-HALF_PI, at);
}

TEST_F(GatlLedFixture, SinOutput) {
  uint8_t output;
  output = gatll::sin::output<double>(-HALF_PI);
  EXPECT_EQ(0x00, output);
  output = gatll::sin::output<double>(0.0);
  EXPECT_EQ(0x7f, output);
  output = gatll::sin::output<double>(HALF_PI);
  EXPECT_EQ(0xfe, output);
  output = gatll::sin::output<double>(PI + HALF_PI);
  EXPECT_EQ(0x00, output);
}

TEST_F(GatlLedFixture, SinOutputMaximum) {
  uint8_t output;
  const uint8_t Maximum = 0x7f;
  output = gatll::sin::output<double>(-HALF_PI, Maximum);
  EXPECT_EQ(0x00, output);
  output = gatll::sin::output<double>(0.0, Maximum);
  EXPECT_EQ(Maximum / 2, output);
  output = gatll::sin::output<double>(HALF_PI, Maximum);
  EXPECT_EQ(Maximum , output);
  output = gatll::sin::output<double>(PI + HALF_PI, Maximum);
  EXPECT_EQ(0x00, output);
}

TEST_F(GatlLedFixture, SinStep) {
  float at = 0.0F;
  const float Step = 0.01F;
  gatll::sin::step(at, Step);
  EXPECT_FLOAT_EQ(Step, at);
  while (at > 0.0F) {
    EXPECT_TRUE(at <= GOS_ARDUINO_LED_SIN_MAXIMUM_AT + Step);
    gatll::sin::step(at, Step);
  }
  EXPECT_FLOAT_EQ(0.0F, at);
}

TEST_F(GatlLedFixture, SinLoop) {
  uint8_t output;
  float at = 0.0F;
  const float Step = 0.01F;
  gatll::sin::step(at, Step);
  EXPECT_FLOAT_EQ(Step, at);
  while (at > 0.0F) {
    output = gatll::sin::output<float>(at);
    EXPECT_CALL(*arduinomock, analogWrite(Pin, output))
      .Times(::testing::Exactly(1));
    gatll::sin::loop<float>(Pin, at, Step);
  }
}

TEST_F(GatlLedFixture, DISABLED_SinFullCycle) {
  std::lock_guard<std::mutex> lock(mutex);
  uint8_t output;
  const double Step = 0.1;
  const double Start = -HALF_PI;
  double at = Start;
  OutputCountMap outputcount;
  while (at < GOS_ARDUINO_LED_SIN_MAXIMUM_AT) {
    output = gatll::sin::output<double>(at);
    OutputCountIterator it = outputcount.find(output);
    if (it == outputcount.end()) {
      outputcount.insert(OutputCountValue(output, 1));
    } else {
      it->second++;
    }
    at += Step;
  }
  for (auto pair : outputcount) {
    EXPECT_CALL(*arduinomock, analogWrite(Pin, pair.first))
      .Times(::testing::Exactly(pair.second));
  }
  EXPECT_CALL(*arduinomock, digitalWrite(Pin, LOW))
    .Times(::testing::Exactly(1));
  gatll::sin::full::cycle<double>(Pin, Start, Step);
}

TEST_F(GatlLedFixture, DISABLED_SinFullCycleCount) {
  std::lock_guard<std::mutex> lock(mutex);
  uint8_t output;
  const uint8_t Count = 6;
  const double Step = 0.1;
  const double Start = -HALF_PI;
  double at = Start;
  uint8_t count = Count;
  OutputCountMap outputcount;
  while (count > 0) {
    while (at < GOS_ARDUINO_LED_SIN_MAXIMUM_AT) {
      output = gatll::sin::output<double>(at);
      OutputCountIterator it = outputcount.find(output);
      if (it == outputcount.end()) {
        outputcount.insert(OutputCountValue(output, 1));
      } else {
        it->second++;
      }
      at += Step;
    }
    at = GOS_ARDUINO_LED_SIN_START;
    --count;
  }
  for (auto pair : outputcount) {
    EXPECT_CALL(*arduinomock, analogWrite(Pin, pair.first))
      .Times(::testing::Exactly(pair.second));
  }
  EXPECT_CALL(*arduinomock, digitalWrite(Pin, LOW))
    .Times(::testing::Exactly(1));
  gatll::sin::full::cycle<double>(Pin, Count, Start, Step);
}

TEST_F(GatlLedFixture, DISABLED_SinFullCycleCountDelay) {
  std::lock_guard<std::mutex> lock(mutex);
  uint8_t delaycount = 0;
  uint8_t output;
  const uint8_t Count = 6;
  const double Step = 0.1;
  const double Start = -HALF_PI;
  const unsigned long Delay = 250;
  double at = Start;
  uint8_t count = Count;
  OutputCountMap outputcount;
  while (count > 0) {
    while (at < GOS_ARDUINO_LED_SIN_MAXIMUM_AT) {
      delaycount++;
      output = gatll::sin::output<double>(at);
      OutputCountIterator it = outputcount.find(output);
      if (it == outputcount.end()) {
        outputcount.insert(OutputCountValue(output, 1));
      } else {
        it->second++;
      }
      at += Step;
    }
    at = GOS_ARDUINO_LED_SIN_START;
    --count;
  }
  for (auto pair : outputcount) {
    EXPECT_CALL(*arduinomock, analogWrite(Pin, pair.first))
      .Times(::testing::Exactly(pair.second));
  }
  EXPECT_CALL(*arduinomock, digitalWrite(Pin, LOW))
    .Times(::testing::Exactly(1));
  EXPECT_CALL(*arduinomock, delay(Delay)).Times(::testing::Exactly(delaycount));
  gatll::sin::full::cycle<double>(Pin, Delay, Count, Start, Step);
}